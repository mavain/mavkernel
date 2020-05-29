extern keyboard_interrupt

global keyboard_interrupt_asm
keyboard_interrupt_asm:
    pusha
    cld
    call keyboard_interrupt
    mov al, 0x20
    out 0x20, al
    popa
    iret
