#include <kernel/port.h>
#include <kernel/tty.h>
#include <kernel/keyboard.h>

#include <stdint.h>

void keyboard_initialize() {
    uint8_t pic_master_mask = inb(0x21);
    outb(0x21, pic_master_mask & 0xFD);
}

void keyboard_interrupt() {

    uint8_t keycode = inb(0x60);
    terminal_writehex8(keycode);

}