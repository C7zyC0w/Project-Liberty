#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void print_str(const char* msg);
void insert_char(char c);
void print_char(char c);
void print_str_color(const char* msg, uint8_t color);
void print_uint(uint32_t n);
void render_input(void);
void auto_scroll(void);

extern volatile uint16_t* vga;
extern int row;
extern int col;
extern int cursor_row;
extern int cursor_col;
extern int input_col;
extern int buf_cursor;

#endif
