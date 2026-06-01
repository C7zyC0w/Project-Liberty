/*
 * Project-Liberty/LibertyOS/kernel/keyboard.c
 *
 * Copyright: C7zyC0w
 */
#include "keyboard.h"


#include <stdint.h>

int shift = 0;
int ctrl = 0;


const char scancode_table[128] = {
    [0x01]=0,     // Escape.
    [0x02]='1', [0x03]='2', [0x04]='3', [0x05]='4',
    [0x06]='5', [0x07]='6', [0x08]='7', [0x09]='8',
    [0x0A]='9', [0x0B]='0', [0x0C]='-', [0x0D]='=',
    [0x0E]=0,     // Backspace (handle separately later).
    [0x0F]='\t',  // Tab.
    [0x10]='q', [0x11]='w', [0x12]='e', [0x13]='r',
    [0x14]='t', [0x15]='y', [0x16]='u', [0x17]='i',
    [0x18]='o', [0x19]='p', [0x1A]='[', [0x1B]=']',
    [0x1C]='\n',  // Enter / Return.
    [0x1E]='a', [0x1F]='s', [0x20]='d', [0x21]='f',
    [0x22]='g', [0x23]='h', [0x24]='j', [0x25]='k',
    [0x26]='l', [0x27]=';', [0x28]='\'', [0x29]='`',
    [0x2B]='\\',
    [0x2C]='z', [0x2D]='x', [0x2E]='c', [0x2F]='v',
    [0x30]='b', [0x31]='n', [0x32]='m', [0x33]=',',
    [0x34]='.', [0x35]='/', [0x39]=' ' // Space.
};

const char scancode_table_shift[128] = {
    [0x02]='!', [0x03]='@', [0x04]='#', [0x05]='$',
    [0x06]='%', [0x07]='^', [0x08]='&', [0x09]='*',
    [0x0A]='(', [0x0B]=')', [0x0C]='_', [0x0D]='+',
    [0x10]='Q', [0x11]='W', [0x12]='E', [0x13]='R',
    [0x14]='T', [0x15]='Y', [0x16]='U', [0x17]='I',
    [0x18]='O', [0x19]='P', [0x1A]='{', [0x1B]='}',
    [0x1C]='\n', //Enter / Return.
    [0x1E]='A', [0x1F]='S', [0x20]='D', [0x21]='F',
    [0x22]='G', [0x23]='H', [0x24]='J', [0x25]='K',
    [0x26]='L', [0x27]=':', [0x28]='"', [0x29]='~',
    [0x2B]='|',
    [0x2C]='Z', [0x2D]='X', [0x2E]='C', [0x2F]='V',
    [0x30]='B', [0x31]='N', [0x32]='M', [0x33]='<',
    [0x34]='>', [0x35]='?', [0x39]=' '
};


void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}


void hide_hw_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}


char scancode_to_char(uint8_t scancode) {
    if (shift) return scancode_table_shift[scancode];
    return scancode_table[scancode];
}


uint8_t inb(uint16_t port) {
    uint8_t result;

    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}




uint16_t user_input() {
    if ((inb(0x64) &1) ==1) {
        uint8_t input = inb(0x60);
        if (input == 0xE0) {
            while ((inb(0x64) &1) !=1) {}
            uint16_t input1 = inb(0x60);
            return (0xE0 << 8) | input1;
        }
        return input;
    } else {return 0;}
}
