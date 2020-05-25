#ifndef __KERNEL_GDT_H
#define __KERNEL_GDT_H

#include <stdint.h>

struct gdt_segment_descriptor {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t type;
    uint8_t flags_and_limit;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_pointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void gdt_create_entry(struct gdt_segment_descriptor* descriptor, uint32_t base, uint32_t limit, uint8_t type);

extern void gdt_load(struct gdt_pointer* gdt_pointer);

#endif