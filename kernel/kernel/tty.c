#include <kernel/tty.h>
#include <drivers/video/vga.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

tty_dev_t tty_dev;

// Initialize the terminal by setting a color and clearing
void terminal_initialize(void)
{
    tty_dev.col = 0;
    tty_dev.row = 0;
    tty_dev.color = get_vga_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    tty_dev.buf = (char*)VIDEO_ADDR;
    terminal_clear();
}

// Clear the display
void terminal_clear(void)
{
    vga_clearscreen();
    tty_dev.col = 0;
    tty_dev.row = 0;
}

// Write a character at the current position
void terminal_putchar(char c)
{
    unsigned char uc = c;
    // TODO: Other control characters
    // Handle newlines
    if (uc == '\n')
    {
        tty_dev.col = 0;
        ++tty_dev.row;
    }
    else
    {
        vga_printchar(uc, tty_dev.col, tty_dev.row, tty_dev.color);
        // Handle line wrapping
        if (++tty_dev.col == MAX_COLS)
        {
            tty_dev.col = 0;
            ++tty_dev.row;
        }
    }

    // Handle scrolling
    if (tty_dev.row == MAX_ROWS)
    {
        // Move all rows up one
        for (int i = 1; i < MAX_ROWS; ++i)
        {
            char *line2 = (char*) &tty_dev.buf[i * MAX_COLS * 2];
            char *line1 = (char*) &tty_dev.buf[(i - 1) * MAX_COLS * 2];
            memcpy(line1, line2, MAX_COLS * 2);
        }

        // // Blank out the screen
        tty_dev.row = MAX_ROWS - 1;
        for (int i = 0; i < MAX_COLS; ++i)
        {
            tty_dev.col = i;
            vga_printchar(' ', tty_dev.col, tty_dev.row, tty_dev.color);
        }
        tty_dev.col = 0;
    }

    vga_updatecursor(tty_dev.col, tty_dev.row);
}

// Write some data to the terminal
void terminal_write(const char *c, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        terminal_putchar(c[i]);
    }
}

// Write a string to the terminal
void terminal_writestring(const char *str)
{
    terminal_write(str, strlen(str));
}

// Newline shortcut
void terminal_newline(void)
{
    terminal_putchar('\n');
}

// Set the terminal color
void terminal_setcolor(enum vga_color fg, enum vga_color bg)
{
    tty_dev.color = get_vga_color(fg, bg);
}

// Set the terminal location
void terminal_setpos(int x, int y)
{
    tty_dev.col = x;
    tty_dev.row = y;
    vga_updatecursor(x, y);
}

int terminal_get_x(void)
{
    return tty_dev.col;
}

int terminal_get_y(void)
{
    return tty_dev.row;
}