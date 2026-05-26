section .multiboot
align 8

header_start:
    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd 0

    dw 0
    dw 0
    dd 8

header_end:

section .text
global start

start:
    mov esp, stack_top

    mov al, 'H'
    mov ah, 0x0E
    int 0x10

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 4096
stack_top:

end:
