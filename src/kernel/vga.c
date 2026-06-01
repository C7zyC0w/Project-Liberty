/*
 * Project-Liberty/LibertyOS/kernel/vga.c
 *
 * Copyright: C7zyC0w
 */
#include "vga.h"


#include <stdint.h>


extern volatile uint16_t* vga = (volatile uint16_t*)0xB8000;



int row = 0;
int col = 0;


void auto_scroll() {
    if (row >= 25) {
        for (int i = 0; i < 24; i++) {
            for (int k = 0; k < 80; k++) {
                vga[i * 80 + k] = vga[(i+1) * 80 + k];
            }
        }
        for (int k = 0; k < 80; k++) {
            vga[24 * 80 + k] = 0x0F20;
        }
        row = 24;
    }
}

void print_str(const char* msg) {
    for (int i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            row++;
            col = 0;
            auto_scroll();
            continue;
        }
        vga[(row * 80) + col] = (uint16_t)((0x0F << 8) | (uint8_t)msg[i]);
        if (col >= 80) {
            row++;
            col = 0;
        }
        col++;
    }
}

void print_char(char c) {
    if (c == '\n') {
        row++;
        col=0;
        auto_scroll();
        return;
    }
    insert_char(c);
    render_input();
    col = input_col + buf_cursor;
}


void draw_cursor() {
    uint16_t cell = vga[(cursor_row * 80) +cursor_col];
    uint8_t ch = cell & 0xFF;
    vga[(cursor_row * 80) + cursor_col] = (uint16_t)((0xF0 << 8) | ch);
}

void erase_cursor() {
    uint16_t cell = vga[(cursor_row * 80) +cursor_col];
    uint8_t ch = cell & 0xFF;
    vga[(cursor_row * 80) + cursor_col] = (uint16_t)((0x0F << 8) | ch);
}


void print_str_color(const char* msg, uint8_t color) {
    for (int i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            row++;
            col = 0;
            auto_scroll();
            continue;
        }
        vga[(row * 80) + col] = (uint16_t)((color << 8) | (uint8_t)msg[i]);
        if (col >= 80) {
            row++;
            col = 0;
        }
        col++;
    }
}


void print_uint(uint32_t n) {
    if (n == 0) {
        print_str("0");
    } else {
        char int_buffer[12];
        int len = 0;
        while (n > 0) {
            int_buffer[len++] = '0' + (n % 10);
            n /= 10;
        }
        for (int i = len - 1; i >= 0; i--) {
            vga[(row * 80) + col] = (uint16_t)((0x0F << 8) | int_buffer[i]);
            col++;
        }
    }
}

