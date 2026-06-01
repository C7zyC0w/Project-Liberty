/*
 * Project-Liberty/LibertyOS/kernel/shell.h
 *
 * Copyright: C7zyC0w
 */

#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

#define HISTORY_MAX 10


extern char input_buf[256];
extern int buf_len;
extern int buf_cursor;

extern int input_col;

extern int cursor_col;
extern int cursor_row;

extern char history[HISTORY_MAX][256];
extern int history_len;
extern int history_index;
extern int history_max;

int str_eq(const char* a, const char* b);
int str_starts_with(const char* str, const char* prefix);

void insert_char(char c);

void render_input(void);

void draw_cursor(void);
void erase_cursor(void);

#endif
