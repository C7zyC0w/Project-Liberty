# Project Liberty / LibertyOS

**Project Liberty** (also referred to as **LibertyOS**) is the codename for a custom Operating System (OS) that I am developing entirely from scratch as an independent project.

The primary goals of Project Liberty are to:

* Learn low-level computer architecture and kernel development
* Gain experience with systems programming and hardware interaction
* Create a lightweight, expandable operating system built with modern C standards
* Explore how operating systems function internally, from boot to user interaction

---

# Current Development Plans

## Boot Process & Kernel

Initially, the OS will use an existing bootloader solution for reliability and compatibility purposes. Current options being considered include:

* **GRUB** — widely supported and highly compatible
* **Limine** — modern, lightweight, and easier for hobby OS development

I have chosen **GRUB 2** as the current bootloader due to its great support, and large compatability.

In the future, I may develop a completely custom bootloader written in Assembly for deeper hardware-level control and understanding.

---

# Programming Languages

Project Liberty will primarily be written in:

* **C23** — used for kernel and system development
* **Assembly (x86_64)** — used for low-level boot and hardware-specific functionality

The operating system will be developed as a **freestanding environment**, meaning it will not rely on standard operating system libraries provided by existing systems like Linux or Windows.

---

# Planned Features

The project will begin with a minimal kernel and gradually expand over time. Planned milestones currently include:

1. Booting into a custom kernel
2. Basic screen output and terminal functionality
3. Keyboard input support
4. Interrupt handling and memory management
5. A basic command-line shell
6. Filesystem support
7. Multitasking and process management
8. Driver support for hardware devices
9. Graphics and windowing experiments
10. Networking support

---

# Long-Term Vision

Project Liberty is intended to evolve from a learning project into a functional hobby operating system with its own architecture, utilities, and potentially its own ecosystem of software.

While the project starts small, the long-term aim is to understand and recreate the core technologies behind modern operating systems through hands-on development.

---

# Development Notes

* Target architecture: **x86_64**
* Primary testing environment: **QEMU virtual machine**
* Build environment: **Linux / Artix Linux (Arch-based Distro)**
* Development approach: incremental and modular

---

# Disclaimer

Project Liberty is currently unfinished, and is not available for mainstream release as of now.
