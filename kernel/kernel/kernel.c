#include <string.h>

#include <drivers/keyboard.h>
#include <drivers/video/vga.h>
#include <kernel/arch.h>
#include <kernel/multiboot.h>
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

void kernel_main(multiboot_info_t *mbd, unsigned int magic)
{
    init_vga();
    terminal_initialize();
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {   
        panic(__FILE__, "BOOT ERROR: Invalid bootloader!", __LINE__);
    }
    terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_writestring("Initializing AtomOS \n");
    if (mbd->flags & MULTIBOOT_INFO_MEMORY)
    {
        terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        terminal_writestring("Memory info: ");
        char *str;
        itoa(mbd->mem_lower, str, 10);
        terminal_writestring(str);
        terminal_writestring(" kiB lower, ");
        itoa(mbd->mem_upper, str, 10);
        terminal_writestring(str);
        terminal_writestring(" kiB upper\n");
    }
    else panic(__FILE__, "BOOT ERROR: No memory information!", __LINE__);
    arch_init();
    init_keyboard();
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("All initialization complete!\n");
    terminal_newline();
    splash_screen();
    
    while (1); // Need this here so we don't stop running
}
