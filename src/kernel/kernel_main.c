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
    gdt_create_entry(&GDT[0], 0, 0x0, 0x0); //Null
    gdt_create_entry(&GDT[1], 0, 0x0, 0x0); //Unused (for some reason?) TODO: Is this actually needed?
    gdt_create_entry(&GDT[2], 0, 0xFFFFFFFF, 0x9A); // Code segment
    gdt_create_entry(&GDT[3], 0, 0xFFFFFFFF, 0x92); // Data segment
    // Declared like this for flat memory addressing

    struct gdt_pointer gdtPass;
    gdtPass.base = (uint32_t)&GDT;
    gdtPass.limit = sizeof(struct gdt_segment_descriptor) * 4 - 1;

    gdt_load(&gdtPass);

    terminal_writestring("GDT loaded...\n");
    terminal_writestring("IDT loading...\n");

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    const uint8_t CODE_SEGMENT_OFFSET = sizeof(struct gdt_segment_descriptor) * 2;

    for(uint16_t i = 0; i < 256; i++) {
        if (i == 21) continue;
        idt_create_entry(&IDT[i], CODE_SEGMENT_OFFSET, idt_handle, 0, IDT_INTERRUPT_GATE); //TODO: Read up on what the fuck im actually doing
    }

    idt_create_entry(&IDT[21], CODE_SEGMENT_OFFSET, keyboard_interrupt_asm, 0, 0x0E);

    struct idt_pointer idtPass;
    idtPass.limit = (sizeof(struct idt_gate_descriptor) * 256) - 1;
    idtPass.base = (uint32_t)&IDT;

    idt_load(&idtPass);

    terminal_writestring("IDT loaded...\n");

    keyboard_initialize();

    //terminal_writehex8(0 / 0);

    //TODO: Why do ISRs work but not IRQs?

    while (1)
    {
        terminal_writestring("Test\n");
    }
}