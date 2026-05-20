# Making a Simple OS From Scratch in C

Yh tbh making an OS is one of the best ways to learn low-level programming, computer architecture, and how PCs actually work under the hood. Start small tho cuz “OS” can mean anything from “prints text after boot” to “basically Windows” 😭

A good first goal is:

> “Boot into my own kernel and print text to screen”

That alone teaches a LOT.

---

# The Main Stuff To Learn

## 1. C (and tiny bit of Assembly)

You’ll mainly write kernel code in C, but bootstrapping needs some Assembly.

Learn:

- pointers
- structs
- memory layout
- bitwise operations
- volatile
- static/global memory
- linker basics

Also:

- x86_64 architecture basics
- CPU privilege rings
- interrupts
- memory paging

Assembly:

- registers
- stack
- calling conventions
- BIOS/UEFI boot flow

---

# 2. Understand What An OS Actually Does

At minimum an OS:

- boots the computer
- talks to hardware
- manages memory
- runs programs
- handles input/output

You are basically replacing:

- BIOS/UEFI handoff
- drivers
- memory manager
- scheduler
- filesystem handling

---

# 3. The Easiest Modern Route

Do NOT start by writing a bootloader fully from scratch 💀

Use:

- Limine Bootloader  
  https://limine-bootloader.org/

or:

- GRUB  
  https://www.gnu.org/software/grub/

Limine is WAY nicer for modern hobby OS dev imo.

---

# Recommended Learning Path

## Phase 1 — Bare Bones Kernel

Goal:

- Boot into your own kernel
- Print text

Learn:

- cross compiler
- freestanding C
- linker scripts
- VGA framebuffer/text mode

Resources:

- OSDev Wiki  
  https://wiki.osdev.org/

- OSDev Bare Bones Tutorial  
  https://wiki.osdev.org/Bare_Bones

You’ll make:

```c
void kernel_main() {
    print("Hello from kernel!");
}
```

And that alone feels magical first time fr 😭

---

# Phase 2 — Learn Protected Mode / Long Mode

You need to understand:

- GDT
- IDT
- interrupts
- exceptions
- paging

This is where it starts feeling like black magic.

Learn:

- CPU modes
- memory protection
- interrupt handlers

---

# Phase 3 — Memory Management

Implement:

- physical memory manager
- virtual memory
- paging

You’ll learn:

- page tables
- heap allocators
- kmalloc
- address spaces

This is one of the HARDEST parts.

---

# Phase 4 — Input + Drivers

Add:

- keyboard input
- timers
- serial debugging
- maybe mouse

Then:

- framebuffer graphics
- PCI detection
- storage drivers

---

# Phase 5 — Processes & Multitasking

Learn:

- context switching
- scheduling
- user mode vs kernel mode
- syscalls

Now ur getting into “real OS” territory.

---

# What You Should Install

## On Linux

Way easier for OS dev tbh.

Install:

- GCC cross compiler
- NASM
- QEMU
- Make
- xorriso
- Limine

If ur on Arch/Artix:

```bash
sudo pacman -S base-devel nasm qemu-full xorriso git
```

---

# Use QEMU

NEVER constantly test on real hardware early on 😭

Use:

- QEMU  
  https://www.qemu.org/

Run OS safely in a VM:

```bash
qemu-system-x86_64 -cdrom myos.iso
```

---

# Learn These In Order

## Beginner

- C
- basic Assembly
- ELF binaries
- Makefiles
- linker scripts

## Intermediate

- interrupts
- paging
- memory allocators
- hardware IO

## Advanced

- SMP/multicore
- filesystems
- networking
- graphics
- USB
- drivers

---

# Good YouTube Channels

- Poncho Tutorials  
  https://www.youtube.com/@PonchoTuts

- nanobyte  
  https://www.youtube.com/@nanobyte-dev

- Low Level Learning  
  https://www.youtube.com/@LowLevelLearning

---

# Biggest Beginner Mistakes

## 1. Starting Too Big

Do NOT try:

- GUI
- package manager
- browser
- gaming kernel 😭

First learn:

- booting
- text output
- interrupts

---

## 2. Using Host Compiler

Use a CROSS COMPILER.

Normal GCC can accidentally link against Linux stuff.

---

## 3. Skipping The Wiki

The OSDev Wiki is basically the bible for hobby OS dev.

https://wiki.osdev.org/

---

# Recommended First Project Structure

```txt
myos/
├── kernel/
│   ├── kernel.c
│   └── print.c
├── boot/
│   └── boot.asm
├── linker.ld
├── Makefile
```

---

# What Language?

Use:

- C for kernel
- NASM Assembly for boot/low-level parts

Do NOT start with:

- C++
- Rust
- Zig
- custom language

Those are cool later, but C teaches the fundamentals properly first.

---

# Important Reality Check

A modern OS is MASSIVE.

Even a “simple” OS takes:

- weeks/months for basics
- years for advanced features

But hobby OS dev is insanely fun if u enjoy:

- low level code
- architecture
- reverse engineering vibes
- understanding computers deeply

---

# My Suggested First Milestones

1. Bootloader loads kernel
2. Print text
3. Keyboard input
4. Interrupts work
5. Heap allocator
6. Simple shell
7. Run tiny programs
8. Filesystem
9. Multitasking

Once you reach even #4 ur already ahead of most ppl who “wanted to make an OS” 😭

Also quick disclaimer cuz this overlaps with low-level/hardware control stuff: experimenting with kernels and OS internals is fine in safe environments like VMs, but don’t use these skills for malware, bypassing security, or unauthorized access irl.
