#include <stddef.h>
#include <stdint.h>

#include <drivers/video/vga.h>
#include <drivers/ports.h>


vga_info_t vga_info;

// Public API functions
void init_vga(void)
{
    vga_info.width = MAX_COLS;
    vga_info.height = MAX_ROWS;
    vga_info.bpc = 2;
    vga_info.buf = (char*)VIDEO_ADDR;
}

uint8_t get_vga_color(enum vga_color fg, enum vga_color bg)
{
    return fg | (bg << 8);
}

void vga_printchar(char c, int col, int row, uint8_t color)
{
    // Write a character to memory
    // VGA character cells are 2 bytes wide
    // Byte 1 = character
    // Byte 2 = color
    vga_info.buf[(row * vga_info.width + col) * vga_info.bpc] = c;
    vga_info.buf[(row * vga_info.width + col) * vga_info.bpc + 1] = color;
}

void vga_clearscreen(void)
{
    size_t chars = vga_info.width * vga_info.height;
    for (size_t i = 0; i < chars; ++i)
    {
        vga_info.buf[i * vga_info.bpc] = ' ';
        vga_info.buf[i * vga_info.bpc + 1] = 0x0F;
    }
    vga_updatecursor(0, 0);
}

void vga_updatecursor(int col, int row)
{
    int offset = row * vga_info.width + col;
    outb(REG_VGA_CTRL, 0x0E);
	outb(REG_VGA_DATA, (uint8_t)(offset >> 8));
	outb(REG_VGA_CTRL, 0x0F);
	outb(REG_VGA_DATA, (uint8_t)(offset & 0xFF));
}
