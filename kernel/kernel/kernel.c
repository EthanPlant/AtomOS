#include <string.h>

#include <drivers/keyboard.h>
#include <drivers/video/vga.h>
#include <kernel/arch.h>
#include <kernel/tty.h>

#define VERSION_STR "June 5, 2021 Build\n"

void splash_screen(void)
{
    terminal_writestring("Welcome to\n");
    terminal_writestring("         _                   ____   _____ \n");
    terminal_writestring("    /\\  | |                 / __ \\ / ____|\n");
    terminal_writestring("   /  \\ | |_ ___  _ __ ___ | |  | | (___  \n");
    terminal_writestring("  / /\\ \\| __/ _ \\| '_ ` _ \\| |  | |\\___ \\ \n");
    terminal_writestring(" / ____ \\ || (_) | | | | | | |__| |____) |\n");
    terminal_writestring("/_/    \\_\\__\\___/|_| |_| |_|\\____/|_____/ \n");
    terminal_setpos(44 - strlen(VERSION_STR), terminal_get_y());
    terminal_writestring(VERSION_STR);
}

void kernel_main(void)
{
    init_vga();
    terminal_initialize();
    terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_writestring("Initializing AtomOS \n");
    arch_init();
    init_keyboard();
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("All initialization complete!\n");
    terminal_newline();
    splash_screen();
    
    while (1); // Need this here so we don't stop running
}
