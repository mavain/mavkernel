PIC1_CTRL   equ 0x20
PIC1_DATA   equ 0x21
PIC2_CTRL   equ 0xA0
PIC2_DATA   equ 0xA1

extern isr_handler_execute

%macro ISR_NOERROR_WITH_ID 1
global isr_handler_%1
isr_handler_%1:
    cli

    push BYTE 0
    push BYTE %1

    jmp isr_common
%endmacro

%macro ISR_ERROR_WITH_ID 1
global isr_handler_%1
isr_handler_%1:
    cli

    push BYTE %1

    jmp isr_common
%endmacro

ISR_NOERROR_WITH_ID 0x00
ISR_NOERROR_WITH_ID 0x01
ISR_NOERROR_WITH_ID 0x02
ISR_NOERROR_WITH_ID 0x03
ISR_NOERROR_WITH_ID 0x04
ISR_NOERROR_WITH_ID 0x05
ISR_NOERROR_WITH_ID 0x06
ISR_NOERROR_WITH_ID 0x07
ISR_ERROR_WITH_ID   0x08
ISR_NOERROR_WITH_ID 0x09
ISR_ERROR_WITH_ID   0x0A
ISR_ERROR_WITH_ID   0x0B
ISR_ERROR_WITH_ID   0x0C
ISR_ERROR_WITH_ID   0x0D
ISR_ERROR_WITH_ID   0x0E
ISR_NOERROR_WITH_ID 0x0F
ISR_NOERROR_WITH_ID 0x10
ISR_NOERROR_WITH_ID 0x11
ISR_NOERROR_WITH_ID 0x12
ISR_NOERROR_WITH_ID 0x13
ISR_NOERROR_WITH_ID 0x14
ISR_NOERROR_WITH_ID 0x15

isr_common:
    pusha   ; Push everything
    mov ax, ds  ; Lower 16 bits of eax = ds
    push eax    ; Save segment descriptor

    mov ax, 0x18 ; Move segments to kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler_execute

    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    add esp, 8

    sti     ; Re-enable interrupts
    iret    ; Return from interrupt

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