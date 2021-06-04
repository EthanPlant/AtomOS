#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <kernel/arch.h>
#include <kernel/tty.h>
#include <drivers/video/vga.h>

#define VERSION_STR "June 4, 2021 Build\n"

void kernel_main(void)
{
    init_vga();
    terminal_initialize();
    arch_init();
    terminal_writestring("Welcome to\n");
    terminal_writestring("         _                   ____   _____ \n");
    terminal_writestring("    /\\  | |                 / __ \\ / ____|\n");
    terminal_writestring("   /  \\ | |_ ___  _ __ ___ | |  | | (___  \n");
    terminal_writestring("  / /\\ \\| __/ _ \\| '_ ` _ \\| |  | |\\___ \\ \n");
    terminal_writestring(" / ____ \\ || (_) | | | | | | |__| |____) |\n");
    terminal_writestring("/_/    \\_\\__\\___/|_| |_| |_|\\____/|_____/ \n");
    terminal_setpos(44 - strlen(VERSION_STR), terminal_get_y());
    terminal_writestring(VERSION_STR);

    while (1); // Need this here so we don't stop running
}
