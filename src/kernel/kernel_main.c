#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/port.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>

static struct gdt_segment_descriptor GDT[4];
static struct idt_gate_descriptor IDT[256];

void kernel_main(void)
{

    terminal_initialize();
    terminal_writestring("MavKernel starting...\n");

    terminal_writestring("GDT loading...\n");

    // First two segment descriptors are null and unused, respectively
    gdt_create_entry(&GDT[0], 0, 0x0, 0x0);         //Null
    gdt_create_entry(&GDT[1], 0, 0x0, 0x0);         //Unused (for some reason?) TODO: Is this actually needed?
    gdt_create_entry(&GDT[2], 0, 0xFFFFFFFF, 0x9A); // Code segment
    gdt_create_entry(&GDT[3], 0, 0xFFFFFFFF, 0x92); // Data segment
    // Declared like this for flat memory addressing

    struct gdt_pointer gdtPass;
    gdtPass.base = (void*)&GDT;
    gdtPass.limit = sizeof(struct gdt_segment_descriptor) * 4 - 1;

    gdt_load(&gdtPass);

    terminal_writestring("GDT loaded...\n");
    terminal_writestring("IDT loading...\n");

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    const uint8_t CODE_SEGMENT_OFFSET = sizeof(struct gdt_segment_descriptor) * 2;

    idt_create_entry(&IDT[0x00], CODE_SEGMENT_OFFSET, isr_handler_0x00, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x01], CODE_SEGMENT_OFFSET, isr_handler_0x01, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x02], CODE_SEGMENT_OFFSET, isr_handler_0x02, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x03], CODE_SEGMENT_OFFSET, isr_handler_0x03, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x04], CODE_SEGMENT_OFFSET, isr_handler_0x04, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x05], CODE_SEGMENT_OFFSET, isr_handler_0x05, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x06], CODE_SEGMENT_OFFSET, isr_handler_0x06, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x07], CODE_SEGMENT_OFFSET, isr_handler_0x07, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x08], CODE_SEGMENT_OFFSET, isr_handler_0x08, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x09], CODE_SEGMENT_OFFSET, isr_handler_0x09, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x0A], CODE_SEGMENT_OFFSET, isr_handler_0x0A, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x0B], CODE_SEGMENT_OFFSET, isr_handler_0x0B, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x0C], CODE_SEGMENT_OFFSET, isr_handler_0x0C, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x0D], CODE_SEGMENT_OFFSET, isr_handler_0x0D, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x0E], CODE_SEGMENT_OFFSET, isr_handler_0x0E, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x0F], CODE_SEGMENT_OFFSET, isr_handler_0x0F, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x10], CODE_SEGMENT_OFFSET, isr_handler_0x10, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x11], CODE_SEGMENT_OFFSET, isr_handler_0x11, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x12], CODE_SEGMENT_OFFSET, isr_handler_0x12, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x13], CODE_SEGMENT_OFFSET, isr_handler_0x13, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x14], CODE_SEGMENT_OFFSET, isr_handler_0x14, 0, IDT_INTERRUPT_GATE);
    idt_create_entry(&IDT[0x15], CODE_SEGMENT_OFFSET, isr_handler_0x15, 0, IDT_INTERRUPT_GATE);

    for (uint16_t i = 0x16; i < 256; i++)
    {
        if (i == 0x21)
            continue;
        idt_create_entry(&IDT[i], CODE_SEGMENT_OFFSET, isr_handler_0x00, 0, IDT_INTERRUPT_GATE); //TODO: Read up on what the fuck im actually doing
    }

    idt_create_entry(&IDT[0x21], CODE_SEGMENT_OFFSET, keyboard_interrupt_asm, 0, IDT_INTERRUPT_GATE);

    struct idt_pointer idtPass;
    idtPass.limit = (sizeof(struct idt_gate_descriptor) * 256) - 1;
    idtPass.base = (void*)&IDT;

    idt_load(&idtPass);

    terminal_writestring("IDT loaded...\n");

    keyboard_initialize();

    //TODO: Why do ISRs work but not IRQs?

    while (1)
    {
        //terminal_writestring("Test\n");
    }
}