#include "vga.h"
#include "../../arch/i386/ports.h"

// Private functions

uint16_t get_offset(int col, int row) { return 2 * (row * MAX_COLS + col) ;}
uint16_t get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
uint16_t get_offset_col(int offset) { return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2; }

// Get the offset of the cursor
uint16_t get_cursor_offset()
{
    port_byte_out(REG_VGA_CTRL, 14);
    uint16_t offset = port_byte_in(REG_VGA_DATA) << 8;
    port_byte_out(REG_VGA_CTRL, 15);
    offset |= port_byte_in(REG_VGA_DATA);
    return offset * 2;
}

// Set the cursor offset
void set_cursor_offset(uint16_t offset)
{
    offset /= 2;
    port_byte_out(REG_VGA_CTRL, 14);
    port_byte_out(REG_VGA_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_VGA_CTRL, 15);
    port_byte_out(REG_VGA_DATA, (uint8_t)(offset & 0xFF));
}

// Print a character at a given position
int print_char(char c, int col, int row, char attr)
{
    unsigned char *vram = (unsigned char*) VIDEO_ADDR;
    if (!attr) attr = WHITE_ON_BLACK;

    // Check if coords are right
    if (col >= MAX_COLS || row > MAX_ROWS)
    {
        vram[2 * (MAX_COLS) * (MAX_ROWS) - 2] = 'E';
        vram[2 * (MAX_COLS) * (MAX_ROWS) - 1] =  RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    // Newline
    if (c == '\n')
    {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    }
    else
    {
        vram[offset] = c;
        vram[offset + 1] = attr;
        offset += 2;
    }

    set_cursor_offset(offset);
    return offset;
}

// Public API

// Clear the screen
void clear_screen()
{
    int screen_size = MAX_COLS * MAX_ROWS;
    
    uint8_t *screen = VIDEO_ADDR;

    for (int i = 0; i < screen_size; ++i)
    {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

// Print a string at a given row and col
void kprint_at(char *msg, int col, int row)
{
    // Set cursor if col/row are negative
    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else
    {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    // Print the message
    int i = 0;
    while (msg[i] != 0)
    {
        offset = print_char(msg[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

// Print a string at the cursor location
void kprint(char *msg)
{
    kprint_at(msg, -1, -1);
}