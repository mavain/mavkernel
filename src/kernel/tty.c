#include <kernel/tty.h>

#include <stdint.h>

#include <string.h>

#include <kernel/port.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_x;
static size_t terminal_y;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void terminal_initialize(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);

    terminal_x = 0;
    terminal_y = 0;
    terminal_color = 15;
    terminal_buffer = VGA_MEMORY;
    for (size_t index = 0; index < VGA_WIDTH * VGA_HEIGHT; index++)
    {
        terminal_buffer[index] = (uint16_t)' ' | (uint16_t)15 << 8;
    }
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_x = 0;
        terminal_y++;
        if (terminal_y == VGA_HEIGHT)
            terminal_y = 0;

        return;
    }

    size_t index = terminal_x + VGA_WIDTH * terminal_y;
    terminal_buffer[index] = (uint16_t)c | terminal_color << 8;

    terminal_x++;
    if (terminal_x == VGA_WIDTH)
    {
        terminal_x = 0;
        terminal_y++;
        if (terminal_y == VGA_HEIGHT)
            terminal_y = 0;
    }
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char *str)
{
    terminal_write(str, strlen(str));
}

void terminal_writehex8(uint8_t n)
{
    terminal_putchar('0');
    terminal_putchar('x');
    uint8_t high = n >> 4;
    uint8_t low = n & 0x0F;
    if (high > 9)
        terminal_putchar(high + 'A' - 10);
    else
        terminal_putchar(high + '0');

    if (low > 9)
        terminal_putchar(low + 'A' - 10);
    else
        terminal_putchar(low + '0');
}

void terminal_writehex32(uint32_t n)
{
    terminal_putchar('0');
    terminal_putchar('x');
    for (uint8_t x = 7; x != 255; x--)
    {
        uint8_t low = (uint8_t)(n >> (x << 2)) & 0xF;
        if (low > 9)
            terminal_putchar(low + 'A' - 10);
        else
            terminal_putchar(low + '0');
    }
}