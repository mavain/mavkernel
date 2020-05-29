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

struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void isr_handler_execute(struct registers regs);

void idt_create_entry(struct idt_gate_descriptor* descriptor, uint16_t code_selector_offset, void(*handler)(), uint8_t privilege_level, uint8_t type);

extern void idt_load(struct idt_pointer* idt_pointer);
extern void isr_handler_0x00();
extern void isr_handler_0x01();
extern void isr_handler_0x02();
extern void isr_handler_0x03();
extern void isr_handler_0x04();
extern void isr_handler_0x05();
extern void isr_handler_0x06();
extern void isr_handler_0x07();
extern void isr_handler_0x08();
extern void isr_handler_0x09();
extern void isr_handler_0x0A();
extern void isr_handler_0x0B();
extern void isr_handler_0x0C();
extern void isr_handler_0x0D();
extern void isr_handler_0x0E();
extern void isr_handler_0x0F();
extern void isr_handler_0x10();
extern void isr_handler_0x11();
extern void isr_handler_0x12();
extern void isr_handler_0x13();
extern void isr_handler_0x14();
extern void isr_handler_0x15();

#endif