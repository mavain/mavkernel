#include <kernel/idt.h>
#include <kernel/tty.h>

void interrupt_handler_execute() {

    terminal_writestring("Interrupt requested: IRQ");

}

void idt_create_entry(struct idt_gate_descriptor* descriptor, uint16_t code_selector_offset, void(*handler)(), uint8_t privilege_level, uint8_t type) {

    const uint8_t IDT_DESC_PRESENT = 0x80;

    descriptor->handler_address_low = (uint32_t)handler & 0xFFFF;
    descriptor->handler_address_high = ((uint32_t)handler >> 16) & 0xFFFF;
    descriptor->gdt_code_segment_selector = code_selector_offset;
    descriptor->access = IDT_DESC_PRESENT | type | ((privilege_level & 3) << 5);
    descriptor->reserved = 0;

}