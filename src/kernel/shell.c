/*
 * Project-Liberty/LibertyOS/kernel/shell.c
 *
 * Copyright: C7zyC0w
 */
#include "shell.h"
#include "vga.h"


#include <stdint.h>

int input_col = 0;

char input_buf[256];
int buf_len = 0;
int buf_cursor = 0;


int cursor_col = 0;
int cursor_row = 0;


#define HISTORY_MAX 10
char history[HISTORY_MAX][256];
int history_len = 0;
int history_index = 0;
int history_max = HISTORY_MAX;



void insert_char(char c) {
    for (int i = buf_len; i >= buf_cursor; i--) {
        input_buf[i+1] = input_buf[i];
    }
    input_buf[buf_cursor] = c;
    buf_cursor++;
    buf_len++;
}


void render_input() {
    col = input_col;
    for (int i = 0; i < buf_len; i++) {
        vga[(row*80) + col] = (uint16_t)((0x0F << 8) | (uint8_t) input_buf[i]);
        col++;
    }
    while (col < (input_col + 76)) {
        vga[(row*80) +col] = (uint16_t)((0x0F << 8) | ' ');
        col++;
    }
}


int str_eq(const char* a, const char* b) {
    for (int i = 0; a[i] || b[i]; i++){
        if (a[i] != b[i]) return 0;
    }
    return 1;
}


int str_starts_with(const char* str, const char* prefix) {
    for (int i = 0; prefix[i] != '\0'; i++){
        if (str[i] != prefix[i]) return 0;
    }
    return 1;
}
