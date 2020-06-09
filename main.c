#include "msp.h"
#include "ST7066U.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Initialize driver
	init_st7066u();

	int i = 0;
    // Delay 160us; count=48
    for(i=0;i<48;i++);

    // Test display
    write_to_display("Hello !!","World !!");


	while(1) {
	    ;
	}


}
