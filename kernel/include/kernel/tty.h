#ifndef _TTY_H
#define _TTY_H

#include <stddef.h>
#include <stdint.h>
#include <drivers/video/vga.h>

// TTY struct
typedef struct tty_dev_s
{
	size_t col;
	size_t row;
    uint8_t color;
    char *buf;

} tty_dev_t;

void terminal_initialize(void);

void terminal_clear(void);

void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_newline(void);

void terminal_setcolor(enum vga_color fg, enum vga_color bg);
void terminal_setpos(int x, int y);

int terminal_get_x(void);
int terminal_get_y(void);

#endif
