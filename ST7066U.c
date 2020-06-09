/*
 *  ST7066U.c
 *
 *  Created on: May 29, 2020
 *  Author: Thomas Brown
 *
 *  Code based on datasheets from the following:
 *
 *  LCD Display Driver:
 *  https://www.newhavendisplay.com/app_notes/ST7066U.pdf
 *
 *  LCD Module:
 *  http://www.newhavendisplay.com/specs/NHD-0208AZ-RN-YBW-33V.pdf
 *
 */
#include "msp432.h"

/**
 *  Initialize ST7066U for interface with MSP432
 */
void init_st7066u() {
    int i = 0;

    // RS P5.0
    // R/W P5.1
    // E P5.2
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
 *  Write to display when it is in 2-line mode
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
 * Sends a command byte to the driver
 */
void command(uint8_t byte) {

    // Configure the data lines as outputs
    P4->DIR = 0xFF;
    // Configure the data lines as outputs
    P4->OUT = byte;

    // RS=0 -> Send Instruction
    P5->OUT &= ~BIT0;
    // R/W=0 -> Write to ST7066U
    P5->OUT &= ~BIT1;

    // E=1 -> Enable bit
    P5->OUT |= BIT2;
    // Delay >= 300ns; count=1500
    int i = 0;
    for (i=0; i<300; i++);
    // E=0 -> Enable bit set to 0
    P5->OUT &= ~BIT2;
}


/**
 * Write to internal RAM
 */
void write(uint8_t byte) {

    // Configure the data lines as outputs
    P4->DIR = 0xFF;
    // Write to Data Lines
    P4->OUT = byte;

    // RS=1 -> Data
    P5->OUT |= BIT0;
    // R/W=0 -> Write to ST7066U
    P5->OUT &= ~BIT1;

    // E=1 -> Enable bit
    P5->OUT |= BIT2;
    // Delay >= 1us; count=300
    int i = 0;
    for (i=0; i<300; i++);
    // E=0 -> Enable bit set to 0
    P5->OUT &= ~BIT2;
}

/**
 * Read from RAM
 */
uint8_t read() {

    // Configure the data lines as inputs
    P4->DIR = 0;

    // RS=1 -> Data
    P5->OUT |= BIT0;
    // R/W=1 -> Read from ST7066U
    P5->OUT |= BIT1;

    // E=1 -> Enable bit
    P5->OUT |= BIT2;
    // Delay >= 1us; count=300
    int i = 0;
    for (i=0; i<300; i++);
    int byte = P4->IN;
    // Disable bit
    P5->OUT &= ~BIT2;

    return byte;

}

/**
 *  Clear display
 *
 *  - Clears all DDRAM by writing "20H" (space code)
 *    to all DDRAM addresses
 *  - Set DDRAM address "00H" into AC.
 *  - Return cursor to the original status, namely
 *    the left edge of the first line of the display.
 *  - Entry mode increment (I/D=1)
 */
void clear_display() {
    command(0x01);
}

/**
 *  Return home
 *
 *  - Set DDRAM address "00H" to AC.
 *  - Return cursor to its original site and return
 *    display to its original status.
 *  - Contents of DDRAM does not change.
 */
void return_home() {
    command(0x02);
}

/**
 *  Entry Mode Set
 *
 *  - Set increment/decrement addressing (cursor or blink).
 *      - I/D=1 address incremented by 1
 *      - I/D=0 address decremented by 1
 *  - Set shift of entire display
 *      - shift=1 and a write operation is performed will shift entire display
 *      - Shift=0 or a read operation will not shift entire display
 */
void entry_mode_set(int inc_dec, int shift) {
    command(0x04 | (inc_dec << 1) | shift);
}

/**
 *  Display On/Off
 *
 *  display: Display on/off control bit
 *  cursor: Cursor on/off control bit
 *  blink: Cursor Blink on/off control bit
 */
void display_on_off(int display, int cursor, int blink) {
    command(0x08 | (display << 2) | (cursor << 1) | blink);
}

/**
 *  Cursor or Display Shift
 *
 *  S/C  |  R/L  |  Description                 |  AC Value
 *   0       0      Shift cursor left               AC=AC-1
 *   0       1      Shift cursor right              AC=AC+1
 *   1       0      Shift display to the left       AC=AC
 *   1       1      Shift display to the right      AC=AC
 */
void cursor_or_display_shift(int shift_cursor, int dir) {
    command(0x10 | (shift_cursor << 3) | (dir << 2));
}

/**
 *  Function Set
 *
 *  DL: Interface data length control bit
 *      - 1/0 => 8-bit/4-bit
 *  N: Display line number control bit
 *      - 1/0 => 2-line/1-line
 *  F: Display font type control bit
 *      - 1/0 => 5x11/5x8
 */
void function_set(int DL, int N, int F) {
    command(0x20 | (DL << 4) | (N << 3) | (F << 2));
}

/**
 *  Set CGRAM Address to AC
 */
void set_cgram_address(uint8_t addr) {
    command(0x40 | (addr & 0x3F));
}

/**
 *  Set DDRAM Address to AC
 *
 *  1-line display mode: DDRAM addresses "00H" to "4FH"
 *  2-line display mode: DDRAM addresses "00H" to "27H" - 1st line
 *                       DDRAM addresses "40H" to "67H" - 2nd line
 */
void set_ddram_address(uint8_t addr) {
    command(0x80 | (addr & 0x7F));
}

/**
 *  Read busy flag (data bus bit 7) and contents of AC
 */
void read_busy_flag(uint8_t addr) {
    read(addr);
}
