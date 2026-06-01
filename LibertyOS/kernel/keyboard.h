/*
 * Project-Liberty/LibertyOS/kernel/keyboard.h
 *
 * Copyright: C7zyC0w
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// ========== keyboard.h ==========
extern const char scancode_table[128];

// ========== keyboard.h ==========
extern const char scancode_table_shift[128];

// ========== keyboard.h ==========
extern int shift;

// ========== keyboard.h ==========
extern int ctrl;

// ========== keyboard.h ==========
char scancode_to_char(uint8_t scancode);

// ========== keyboard.h ==========
uint16_t user_input(void);

// ========== keyboard.h ==========
void hide_hw_cursor(void);

// ========== keyboard.h ==========
uint8_t inb(uint16_t port);

// ========== keyboard.h ==========
void outb(uint16_t port, uint8_t value);

#endif
