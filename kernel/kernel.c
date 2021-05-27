#include "arch/i386/ports.h"

void kernel_main()
{
    port_byte_out(0x3D4, 14); // Request high byte of cursor pos
    int pos = port_byte_in(0x3D5) << 8;
    port_byte_out(0x3D4, 15); // Request low byte of cursor pos
    pos |= port_byte_in(0x3D5);

    // VGA cells consist of 2 bytes
    int offset_from_vga = pos * 2;

    // Write a character to the cursor position
    char *vga = 0xB8000;
    vga[offset_from_vga] = 'X'; // Write the character
    vga[offset_from_vga + 1] = 0x0F; // White on black
}