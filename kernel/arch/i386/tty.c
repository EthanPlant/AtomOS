#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include <vga.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const uint16_t *VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_col;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_col = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; ++y)
    {
        for (size_t x = 0; x < VGA_WIDTH; ++x) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    unsigned char uc = c;
    // Newline support
    if (uc == '\n')
    {
        terminal_col = 0;
        ++terminal_row;
        if (terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
    else
    {
        terminal_putentryat(uc, terminal_color, terminal_col, terminal_row);
        if (++terminal_col == VGA_WIDTH)
        {
            terminal_col = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}

void terminal_setlocation(size_t x, size_t y)
{
    terminal_col = x;
    terminal_row = y;
}

int terminal_getrow(void)
{
    return terminal_row;
}