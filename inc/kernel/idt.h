#ifndef __KERNEL_IDT_H
#define __KERNEL_IDT_H

#include <stdint.h>

struct idt_gate_descriptor {
    uint16_t handler_address_low;
    uint16_t gdt_code_segment_selector;
    uint8_t reserved;
    uint8_t access;
    uint16_t handler_address_high;
} __attribute__((packed));

struct idt_pointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void interrupt_handler_execute();

void idt_create_entry(struct idt_gate_descriptor* descriptor, uint16_t code_selector_offset, void(*handler)(), uint8_t privilege_level, uint8_t type);

extern void idt_load(struct idt_pointer* idt_pointer);
extern void idt_handle();

#endif