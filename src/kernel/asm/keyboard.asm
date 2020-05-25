extern keyboard_interrupt

global keyboard_interrupt_asm
keyboard_interrupt_asm:
    pushad
    cld
    call keyboard_interrupt
    mov al, 0x20
    out 0x20, al
    popad
    iretd
