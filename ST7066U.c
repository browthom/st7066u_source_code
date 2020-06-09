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
 *
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
 *
 */
void write(uint8_t byte) {

    // Configure the data lines as outputs
    P4->DIR = 0xFF;
    // Write to Data Lines
    P4->OUT = byte;

    // RS=1 -> Send Data
    P5->OUT |= BIT0;
    // R/W=0 -> Write to ST7066U
    P5->OUT &= ~BIT1;

    // E=1 -> Enable bit
    P5->OUT |= BIT2;
    // Delay >= 500ns; count~=1500
    int i = 0;
    for (i=0; i<300; i++);
    // E=0 -> Enable bit set to 0
    P5->OUT &= ~BIT2;
}

/**
 *
 */
uint8_t read() {

    // Configure the data lines as inputs
    P4->DIR = 0;
    // Register Select signal = Data
    P5->OUT |= BIT0;
    // Read from LCD
    P5->OUT |= BIT1;
    // Enable bit
    P5->OUT |= BIT2;
    // Delay >= 300ns
    int byte = P4->IN;
    // Disable bit
    P5->OUT &= ~BIT2;

    return byte;

}

void clear_display() {
    command(0x01);
}

void return_home() {
    command(0x02);
}

void entry_mode_set(int inc_dec, int shift) {
    command(0x04 | (inc_dec << 1) | shift);
}

void display_on_off(int display, int cursor, int cursor_pos) {
    command(0x08 | (display << 2) | (cursor << 1) | cursor_pos);
}

void cursor_or_display_shift(int set_cursor, int dir) {
    command(0x10 | (set_cursor << 3) | (dir << 2));
}

void function_set(int interface_data, int number_of_lines, int font_size) {
    command(0x20 | (interface_data << 4) | (number_of_lines << 3) | (font_size << 2));
}

void set_cgram_address(uint8_t addr) {
    command(0x40 | (addr & 0x3F));
}

void set_ddram_address(uint8_t addr) {
    command(0x80 | (addr & 0x7F));
}

void read_busy_flag(uint8_t addr) {

}

void write_data_to_ram(uint8_t addr) {
    write(addr);
}

void read_data_from_ram(uint8_t addr) {
   // read(addr);
}
