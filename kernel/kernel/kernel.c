#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <kernel/arch.h>
#include <kernel/tty.h>

void kernel_main(void)
{
    char* version_string = "June 1, 2021 Build\n";
    terminal_initialize();
    arch_init();
    printf("Welcome to\n");
    printf("         _                   ____   _____ \n");
    printf("    /\\  | |                 / __ \\ / ____|\n");
    printf("   /  \\ | |_ ___  _ __ ___ | |  | | (___  \n");
    printf("  / /\\ \\| __/ _ \\| '_ ` _ \\| |  | |\\___ \\ \n");
    printf(" / ____ \\ || (_) | | | | | | |__| |____) |\n");
    printf("/_/    \\_\\__\\___/|_| |_| |_|\\____/|_____/ \n");
    terminal_setlocation(80 - strlen(version_string) - 1, terminal_getrow());
    terminal_writestring(version_string);
}
