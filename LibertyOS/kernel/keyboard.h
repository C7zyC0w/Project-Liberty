/*
 * Project-Liberty/LibertyOS/kernel/keyboard.h
 *
 * Copyright: C7zyC0w
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern const char scancode_table[128];
extern const char scancode_table_shift[128];

extern int shift;
extern int ctrl;

char scancode_to_char(uint8_t scancode);

uint16_t user_input(void);

void hide_hw_cursor(void);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

#endif
