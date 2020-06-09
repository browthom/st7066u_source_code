#include "msp.h"
#include "ST7066U.h"

// RS P5.0
// R/W P5.1
// E P5.2


/**
 *
 */
void init_st7066u() {
    int i = 0;
    P5->DIR |= BIT0 | BIT1 | BIT2;
    P5->OUT &= ~BIT0 & ~BIT1 & ~BIT2;
    P4->DIR = 1;
    P4->OUT = 0;

    // E=0 -> Enable bit set to 0
    P5->OUT &= ~BIT2;
    // Delay >40ms; count=120,000
    for(i=0;i<12000;i++);
    // Wake up (Function set DL=1; 8 bits)
    command(0x30);          // 0011 0000
    // Delay 5ms; count=15,000
    for(i=0;i<1500;i++);
    // Wake up #2
    command(0x30);          // 0011 0000
    // Delay 160us; count=48
    for(i=0;i<48;i++);
    // wake up #3
    command(0x30);          // 0011 0000
    // Delay 160us; count=48
    for(i=0;i<48;i++);
    // Function set: 8-bit/2-line/5x8 chars
    command(0x38);          // 0011 1000
    // Delay 160us; count=48
    for(i=0;i<48;i++);
    // Set cursor   (C/L)
    command(0x10);          // 0000 0001
    // Delay 160us; count=48
    for(i=0;i<48;i++);
    // Display ON; Cursor ON
    command(0x0F);          // 0000 1100
    // Delay 160us; count=48
    for(i=0;i<48;i++);
    // Entry mode set
    command(0x06);          // 0000 0110
}

/**
 *
 */
void write_to_display(uint8_t* line_1, uint8_t* line_2) {
    // set DDRAM address
    command(0x80);                  // 1000 0000

    int i = 7, j=0;

    for (i = 7; i >= 0; i--) {
        for(j=0;j<48;j++);
        write(*line_1);
        line_1++;
    }

    for(j=0;j<48;j++);

    // set DDRAM address
    command(0xC0);                  // 1100 0000

    for (i = 7; i >= 0; i--) {
        for(j=0;j<48;j++);
        write(*line_2);
        line_2++;
    }
}

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P2->DIR |= BIT0;
	P2->OUT &= ~BIT0;

	init_st7066u();

	int i = 0;
    // Delay 160us; count=48
    for(i=0;i<48;i++);

    write_to_display("test1234","test5678");    // 0110 0001 -> 'a'


	while(1) {
	    ;
	}


}
