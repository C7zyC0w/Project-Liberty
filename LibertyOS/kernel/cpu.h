/*
 * Project-Liberty/LibertyOS/kernel/cpu.h
 *
 * Copyright: C7zyC0w
 */
#ifndef CPU_H
#define CPU_H

#include <stdint.h>

extern char cpu_vendor[13];
extern char cpu_brand[49];

void get_cpu_vendor(char* vendor);
void get_cpu_brand(char* brand);

#endif
