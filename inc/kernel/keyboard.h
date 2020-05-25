#ifndef __KERNEL_KEYBOARD_H
#define __KERNEL_KEYBOARD_H

void keyboard_initialize();

void keyboard_interrupt();

extern void keyboard_interrupt_asm();

#endif