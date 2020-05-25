PIC1_CTRL   equ 0x20
PIC1_DATA   equ 0x21
PIC2_CTRL   equ 0xA0
PIC2_DATA   equ 0xA1

extern interrupt_handler_execute

global idt_handle
idt_handle:
    pushad
    cld
    call interrupt_handler_execute
    popad
    iretd

global idt_load
idt_load:
    push ebp
    mov ebp, esp

    ; Set up the PICs to work with IRQ as well as ISRs
    mov al, 0x11        ; Tell PICs to wait for commands
    out PIC1_CTRL, al
    out PIC2_CTRL, al

    mov al, 0x20
    out PIC1_DATA, al   ; Remap first PIC to the 0x20-0x27 interrupts
    mov al, 0x28
    out PIC2_DATA, al   ; Remap second PIC to the 0x28-0x30 interrupts

    mov al, 0x00
    out PIC1_DATA, al   ; Set up cascading
    out PIC2_DATA, al

    mov al, 0x01
    out PIC1_DATA, al   ; Set up environment information
    out PIC2_DATA, al

    mov al, 0xFF
    out PIC1_DATA, al   ; Masking interrupts
    out PIC2_DATA, al

    ; Finally load the IDT
    mov eax, [ebp+8]
    cli
    lidt [eax]
    sti

    mov esp, ebp
    pop ebp
    ret