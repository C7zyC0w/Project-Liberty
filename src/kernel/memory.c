/*
 * Project-Liberty/LibertyOS/kernel/memory.c
 *
 * Copyright: C7zyC0w
 */
#include "memory.h"


#include <stdint.h>

uint32_t get_total_mem_mb(uint32_t* mb_info) {
    uint8_t* ptr = (uint8_t*)mb_info + 8;
    uint32_t total_size = mb_info[0];
    uint8_t* end = (uint8_t*)mb_info + total_size;
    uint32_t total_kb = 0;

    while (ptr < end) {
        uint32_t type = *(uint32_t*)ptr;
        uint32_t size = *(uint32_t*)(ptr + 4);

        if (type == 6) {
            uint8_t* entry = ptr + 16;
            while (entry < ptr + size) {
                uint32_t entry_type = *(uint32_t*)(entry + 16);
                uint32_t len_low = *(uint32_t*)(entry + 8);
                if (entry_type == 1) {
                    total_kb += len_low /1024;
                }
                entry += *(uint32_t*)(entry + 20) ? *(uint32_t*)(entry + 20) : 24;
            }
        }
        if (type == 0) break;
        ptr += (size + 7) & ~7;
    }
    return total_kb / 1024;
}
