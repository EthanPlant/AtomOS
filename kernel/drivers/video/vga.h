#ifndef VGA_H
#define VGA_H

#define VIDEO_ADDR 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F
#define RED_ON_WHITE 0xF4

#define REG_VGA_CTRL 0x3D4
#define REG_VGA_DATA 0x3D5

void clear_screen();
void kprint_at(char *msg, int col, int row);
void kprint(char *msg);

#endif