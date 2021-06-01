#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <kernel/arch.h>
#include <kernel/tty.h>
#include <drivers/video/vga.h>

#define VERSION_STR "May 31, 2021 Build\n"

void kernel_main(void)
{
    init_vga();
    terminal_initialize();
    arch_init();
    printf("Welcome to\n");
    printf("         _                   ____   _____ \n");
    printf("    /\\  | |                 / __ \\ / ____|\n");
    printf("   /  \\ | |_ ___  _ __ ___ | |  | | (___  \n");
    printf("  / /\\ \\| __/ _ \\| '_ ` _ \\| |  | |\\___ \\ \n");
    printf(" / ____ \\ || (_) | | | | | | |__| |____) |\n");
    printf("/_/    \\_\\__\\___/|_| |_| |_|\\____/|_____/ \n");
    terminal_setpos(43 - strlen(VERSION_STR), terminal_get_y());
    printf(VERSION_STR);
}
