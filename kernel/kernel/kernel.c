#include <stdio.h>
#include <stdlib.h>

#include <kernel/arch.h>
#include <kernel/tty.h>

void kernel_main(void)
{
    terminal_initialize();
    arch_init();
    printf("Welcome to\n");
    printf("         _                   ____   _____ \n");
    printf("    /\\  | |                 / __ \\ / ____|\n");
    printf("   /  \\ | |_ ___  _ __ ___ | |  | | (___  \n");
    printf("  / /\\ \\| __/ _ \\| '_ ` _ \\| |  | |\\___ \\ \n");
    printf(" / ____ \\ || (_) | | | | | | |__| |____) |\n");
    printf("/_/    \\_\\__\\___/|_| |_| |_|\\____/|_____/ \n");
    printf("May 28, 2021 Build\n");
}
