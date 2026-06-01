/*
 * Project-Liberty/LibertyOS/kernel/cpu.h
 *
 * Copyright: C7zyC0w
 */
#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// ========== cpu.h ==========
extern char cpu_vendor[13];

// ========== cpu.h ==========
extern char cpu_brand[49];

// ========== cpu.h ==========
void get_cpu_vendor(char* vendor);

// ========== cpu.h ==========
void get_cpu_brand(char* brand);

#endif
