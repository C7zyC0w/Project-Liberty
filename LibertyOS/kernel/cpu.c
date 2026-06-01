/*
 * Project-Liberty/LibertyOS/kernel/cpu.c
 *
 * Copyright: C7zyC0w
 */
#include "cpu.h"

#include <stdint.h>

char cpu_vendor[13];
char cpu_brand[49];


void get_cpu_vendor(char* vendor) {
    uint32_t ebx, ecx, edx;
    // leaf0:
    __asm__ volatile (
        "cpuid"
        : "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0)
    );
    // ebx:
    vendor[0] = ebx & 0x0FF;
    vendor[1] = (ebx >> 8) & 0x0FF;
    vendor[2] = (ebx >> 16) & 0x0FF;
    vendor[3] = (ebx >> 24) & 0x0FF;
    // edx:
    vendor[4] = edx & 0x0FF;
    vendor[5] = (edx >> 8) & 0x0FF;
    vendor[6] = (edx >> 16) & 0x0FF;
    vendor[7] = (edx >> 24) & 0x0FF;
    // ecx:
    vendor[8] = ecx & 0x0FF;
    vendor[9] = (ecx >> 8) & 0x0FF;
    vendor[10] = (ecx >> 16) & 0x0FF;
    vendor[11] = (ecx >> 24) & 0x0FF;
    // End:
    vendor[12] = '\0';
}


void get_cpu_brand(char* brand) {
    uint32_t eax, ebx, ecx, edx;
    // leaf1:
    __asm__ volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0x80000002)
    );
    // eax:
    brand[0] = eax & 0x0FF;
    brand[1] = (eax >> 8) & 0x0FF;
    brand[2] = (eax >> 16) & 0x0FF;
    brand[3] = (eax >> 24) & 0x0FF;
    // ebx:
    brand[4] = ebx & 0x0FF;
    brand[5] = (ebx >> 8) & 0x0FF;
    brand[6] = (ebx >> 16) & 0x0FF;
    brand[7] = (ebx >> 24) & 0x0FF;
    // ecx:
    brand[8] = ecx & 0x0FF;
    brand[9] = (ecx >> 8) & 0x0FF;
    brand[10] = (ecx >> 16) & 0x0FF;
    brand[11] = (ecx >> 24) & 0x0FF;
    // edx:
    brand[12] = edx & 0x0FF;
    brand[13] = (edx >> 8) & 0x0FF;
    brand[14] = (edx >> 16) & 0x0FF;
    brand[15] = (edx >> 24) & 0x0FF;

    // leaf2:
    __asm__ volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0x80000003)
    );
    // eax:
    brand[16] = eax & 0x0FF;
    brand[17] = (eax >> 8) & 0x0FF;
    brand[18] = (eax >> 16) & 0x0FF;
    brand[19] = (eax >> 24) & 0x0FF;
    // ebx:
    brand[20] = ebx & 0x0FF;
    brand[21] = (ebx >> 8) & 0x0FF;
    brand[22] = (ebx >> 16) & 0x0FF;
    brand[23] = (ebx >> 24) & 0x0FF;
    // ecx:
    brand[24] = ecx & 0x0FF;
    brand[25] = (ecx >> 8) & 0x0FF;
    brand[26] = (ecx >> 16) & 0x0FF;
    brand[27] = (ecx >> 24) & 0x0FF;
    //edx:
    brand[28] = edx & 0x0FF;
    brand[29] = (edx >> 8) & 0x0FF;
    brand[30] = (edx >> 16) & 0x0FF;
    brand[31] = (edx >> 24) & 0x0FF;

    // leaf3:
    __asm__ volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(0x80000004)
    );
    // eax:
    brand[32] = eax & 0x0FF;
    brand[33] = (eax >> 8) & 0x0FF;
    brand[34] = (eax >> 16) & 0x0FF;
    brand[35] = (eax >> 24) & 0x0FF;
    // ebx:
    brand[36] = ebx & 0x0FF;
    brand[37] = (ebx >> 8) & 0x0FF;
    brand[38] = (ebx >> 16) & 0x0FF;
    brand[39] = (ebx >> 24) & 0x0FF;
    // ecx:
    brand[40] = ecx & 0x0FF;
    brand[41] = (ecx >> 8) & 0x0FF;
    brand[42] = (ecx >> 16) & 0x0FF;
    brand[43] = (ecx >> 24) & 0x0FF;
    // edx:
    brand[44] = edx & 0x0FF;
    brand[45] = (edx >> 8) & 0x0FF;
    brand[46] = (edx >> 16) & 0x0FF;
    brand[47] = (edx >> 24) & 0x0FF;
    // End:
    brand[48] = '\0';
}
