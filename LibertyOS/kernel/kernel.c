#include <stdint.h>

void kernel_main() {
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = 0x0F20;
    }

    const char msg[] = "Hello, World!";
    int len = 13;
    for (int i = 0; i < len; i++) {
        vga[i] = (uint16_t)((0x0F << 8) | (uint8_t)msg[i]);
    }

    while (1) {}
}