#include <kernel/tty.h>
#include <kernel/arch.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <drivers/video/vga.h>
#include <stdio.h>

int arch_init(void)
{
    terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_writestring("Initializing AtomOS \n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("Initializing GDT... ");
    init_gdt();
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("done\n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("Initializing IDT... ");
    init_idt();
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("done\n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("Installing ISRs... ");
    isr_install();
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("done\n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("All initialization complete!\n");
    return 0;
}