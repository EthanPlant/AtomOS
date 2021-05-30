#ifndef _TTY_H
#define _TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_setcolor(uint8_t color);
void terminal_setlocation(size_t x, size_t y);

int terminal_getrow(void);

#endif
