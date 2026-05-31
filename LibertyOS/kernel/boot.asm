global start
extern kernel_main

section .multiboot2
align 8
    dd 0xE85250D6
    dd 0
    dd 16
    dd 0x100000000 - (0xE85250D6 + 0 + 16)

    dw 0
    dw 0
    dd 8

section .text

start:
    mov byte [0xB8000], 'A'
    mov byte [0xB8001], 0x0F

    push ebx
    call kernel_main

hang:
    cli
    hlt
    jmp hang
