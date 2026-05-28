#include <stdint.h>


volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

int row = 0;
int col = 0;

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
    [0x1C]='\n',  // Enter
    [0x1E]='a', [0x1F]='s', [0x20]='d', [0x21]='f',
    [0x22]='g', [0x23]='h', [0x24]='j', [0x25]='k',
    [0x26]='l', [0x27]=';', [0x28]='\'', [0x29]='`',
    [0x2B]='\\',
    [0x2C]='z', [0x2D]='x', [0x2E]='c', [0x2F]='v',
    [0x30]='b', [0x31]='n', [0x32]='m', [0x33]=',',
    [0x34]='.', [0x35]='/', [0x39]=' '
};

static inline void outb(uint16_t port, uint8_t value) {  
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}


void hide_hw_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}


char scancode_to_char(uint8_t scancode) {
    return scancode_table[scancode];
}

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

uint8_t user_input() {
    while ((inb(0x64) & 1 )!= 1) {}
    uint8_t input = inb(0x60);
    return input;
}

void print_str(const char* msg) {
    for (int i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            row++;
            col = 0;
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
        return;
    }
    vga[(row * 80) + col] = (uint16_t)((0x0F << 8) | (uint8_t)c);
    col++;
}

void draw_cursor() {
    vga[(row * 80) + col] = (uint16_t)((0x0F << 8) | 0xDB);
}

void erase_cursor() {
    vga[(row * 80) + col] = (uint16_t)((0x0F << 8) | ' ');
}

void kernel_main() {


    for (int i = 0; i < 80 * 25; i++) vga[i] = 0x0F20;
    
    hide_hw_cursor();
    
    const char msg[] = "========== LibertyOS Version: Pre-Alpha ==========";
    
    print_str(msg);
    print_str("\n\n\n>>> ");
    draw_cursor();
    
    
    while (1) {
        uint8_t key = user_input();
        char c = scancode_to_char(key);
        
        if (key >= 0x80 || c == 0) continue;
        
        erase_cursor();
        print_char(c);
        
        if (c == '\n') print_str(">>> ");
      
        draw_cursor();
    }
}