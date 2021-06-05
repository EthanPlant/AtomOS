#include <drivers/keyboard.h>
#include <isr.h>
#include <kernel/tty.h>
#include <stdint.h>
#include <drivers/ports.h>
#include <drivers/video/vga.h>
#include <stdbool.h>

const char ascii_table[] = 
{
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

const char shift_ascii_table[] = 
{
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

bool is_left_shift;
bool is_right_shift;

char translate(uint8_t scancode, bool uppercase)
{
    if (scancode > 58) return 0;
    
    if (uppercase) return shift_ascii_table[scancode];
    
    else return ascii_table[scancode];
}

static void keyboard_callback(registers_t *regs)
{
    //terminal_writestring("Keyboard Event\n");
    uint8_t scancode = inb(0x60);
    switch (scancode)
    {
        case BACKSPACE:
            terminal_backspace();
            return;
        case ENTER:
            terminal_newline();
            return;
        case LEFT_SHIFT:
            is_left_shift = true;
            return;
        case LEFT_SHIFT + 0x80:
            is_left_shift = false;
            return;
        case RIGHT_SHIFT:
            is_right_shift = true;
            return;
        case RIGHT_SHIFT + 0x80:
            is_right_shift = false;
            return;
    }

    char ascii = translate(scancode, (is_left_shift || is_right_shift));
    if (ascii) terminal_putchar(ascii);
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