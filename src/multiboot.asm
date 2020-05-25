MB_MBALIGN      equ 1 << 0
MB_MEMINFO      equ 1 << 1
MB_FLAGS        equ MB_MBALIGN | MB_MEMINFO
MB_MAGIC        equ 0x1BADB002
MB_CHECKSUM        equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section .text
global kernel_start:function (kernel_start.end - kernel_start)
kernel_start:
    mov esp, stack_top

    extern kernel_main
    call kernel_main

    cli
.hang: 
    hlt
    jmp .hang
.end: