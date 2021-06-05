#include <drivers/keyboard.h>
#include <isr.h>
#include <kernel/tty.h>
#include <stdint.h>
#include <drivers/ports.h>
#include <drivers/video/vga.h>

static void keyboard_callback(registers_t *regs)
{
    terminal_writestring("Keyboard Event\n");
    uint8_t scancode = inb(0x60);
}

void init_keyboard(void)
{
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("Initializing keyboard... ");
    register_interrupt_handler(IRQ1, keyboard_callback);
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("done\n");
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}