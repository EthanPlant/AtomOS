#include <stdio.h>

#include <kernel/arch.h>

#include "include/gdt.h"
#include "include/idt.h"

extern void int_test();

int arch_init(void)
{
    init_gdt();
    printf("GDT Sucessfully loaded\n");
    init_idt();
    printf("Idt initialized\n");
    int_test();
    return 0;
}