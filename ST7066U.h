/*
 * ST7066U.h
 *
 *  Created on: May 29, 2020
 *      Author: browt
 */

#ifndef ST7066U_H_
#define ST7066U_H_


/**
 *  Initialize ST7066U for interface with MSP432
 */
void init_st7066u();

/**
 *  Write to display when it is in 2-line mode
 */
void write_to_display(uint8_t* line_1, uint8_t* line_2);

/**
 * Sends a command byte to the driver
 */
void command(uint8_t byte);

/**
 * Write to internal RAM
 */
void write(uint8_t byte);

/**
 * Read from RAM
 */
uint8_t read();

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
void clear_display();

/**
 *  Return home
 *
 *  - Set DDRAM address "00H" to AC.
 *  - Return cursor to its original site and return
 *    display to its original status.
 *  - Contents of DDRAM does not change.
 */
void return_home();

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
void entry_mode_set(int inc_dec, int shift);

/**
 *  Display On/Off
 *
 *  display: Display on/off control bit
 *  cursor: Cursor on/off control bit
 *  blink: Cursor Blink on/off control bit
 */
void display_on_off(int display, int cursor, int blink);

/**
 *  Cursor or Display Shift
 *
 *  S/C  |  R/L  |  Description                 |  AC Value
 *   0       0      Shift cursor left               AC=AC-1
 *   0       1      Shift cursor right              AC=AC+1
 *   1       0      Shift display to the left       AC=AC
 *   1       1      Shift display to the right      AC=AC
 */
void cursor_or_display_shift(int shift_cursor, int dir);

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
void function_set(int DL, int N, int F);

/**
 *  Set CGRAM Address to AC
 */
void set_cgram_address(uint8_t addr);

/**
 *  Set DDRAM Address to AC
 *
 *  1-line display mode: DDRAM addresses "00H" to "4FH"
 *  2-line display mode: DDRAM addresses "00H" to "27H" - 1st line
 *                       DDRAM addresses "40H" to "67H" - 2nd line
 */
void set_ddram_address(uint8_t addr);

/**
 *  Read busy flag (data bus bit 7) and contents of AC
 */
void read_busy_flag(uint8_t addr);


#endif /* ST7066U_H_ */
