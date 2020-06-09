/*
 * ST7066U.h
 *
 *  Created on: May 29, 2020
 *      Author: browt
 */

#ifndef ST7066U_H_
#define ST7066U_H_

void command(uint8_t byte);
void write(uint8_t byte);
uint8_t read();

void clear_display();
void return_home();
void entry_mode_set(int inc_dec, int shift);
void display_on_off(int display, int cursor, int cursor_pos);
void cursor_or_display_shift(int set_cursor, int dir);
void function_set(int interface_data, int number_of_lines, int font_size);
void set_cgram_address(uint8_t addr);
void set_ddram_address(uint8_t addr);
void read_busy_flag(uint8_t addr);
void write_data_to_ram(uint8_t addr);
void read_data_from_ram(uint8_t addr);

#endif /* ST7066U_H_ */
