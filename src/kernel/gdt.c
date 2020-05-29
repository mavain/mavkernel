#include <kernel/gdt.h>

#include <stdint.h>

void gdt_create_entry(struct gdt_segment_descriptor *descriptor, uint32_t base, uint32_t limit, uint8_t type)
{

    uint8_t *target = (uint8_t *)descriptor;

    if (limit <= 0x10000)
    {
        target[6] = 0x40;
    }
    else
    {
        if ((limit & 0xFFF) != 0xFFF)
        {
            limit = (limit >> 12) - 1;
        }
        else
        {
            limit = limit >> 12;
        }
        target[6] = 0xC0;
    }

    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    target[5] = type;
}