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
    printf("Initializing AtomOS \n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf("Initializing GDT... ");
    init_gdt();
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("done\n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf("Initializing IDT... ");
    init_idt();
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("done\n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf("Installing ISRs... ");
    isr_install();
    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    printf("done\n");

    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printf("All initialization complete!\n");
    return 0;
}