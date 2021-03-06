#ifndef _VGA_H
#define _VGA_H

// Simple VGA text mode driver

#include <stdint.h>

#define VIDEO_ADDR 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define REG_VGA_CTRL 0x3D4
#define REG_VGA_DATA 0x3D5

enum vga_color 
{
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_PINK = 13,
	VGA_COLOR_YELLOW = 14,
	VGA_COLOR_WHITE = 15,
};

typedef struct vga_info_s
{
	uint16_t width;
	uint16_t height;
	unsigned int bpc;
	char* buf;
} vga_info_t;

void init_vga(void);

uint8_t get_vga_color(enum vga_color fg, enum vga_color bg);

void vga_printchar(char c, int col, int row, uint8_t color);
void vga_clearscreen(void);

void vga_updatecursor(int col, int row);

#endif