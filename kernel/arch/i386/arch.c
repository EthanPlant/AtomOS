#include <stdio.h>

#include <kernel/arch.h>

#include "include/gdt.h"

int arch_init(void)
{
    init_gdt();
    printf("GDT Sucessfully loaded\n");
    return 0;
}