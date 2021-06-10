#include <stdio.h>
#include <stdint.h>

#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/isr.h>

#include <kernel/arch.h>
#include <kernel/timer.h>
#include <kernel/tty.h>
#include <kernel/utils.h>

int arch_init(void)
{
    init_gdt();
    init_idt();
    isr_install();
    init_timer(TIMER_FREQ);


    return 0;
}

void print_registers(void)
{
    uint16_t cs, ds, es, fs, gs;
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, esp, ebp;

    char *str = "";

    // Get all of our registers
    // Need to use asm for this sadly :(
    // Copy eax first as we'll use it as our working register
    asm volatile("movl %%eax, %%eax": "=a" (eax) : );
    asm volatile("movl %%cs, %%eax" : "=a" (cs) : );
    asm volatile("movl %%ds, %%eax" : "=a" (ds) : );
    asm volatile("movl %%es, %%eax" : "=a" (es) : );
    asm volatile("movl %%fs, %%eax" : "=a" (fs) : );
    asm volatile("movl %%gs, %%eax" : "=a" (gs) : );
    asm volatile("movl %%ebx, %%eax": "=a" (ebx) : );
    asm volatile("movl %%ecx, %%eax": "=a" (ecx) : );
    asm volatile("movl %%edx, %%eax": "=a" (edx) : );
    asm volatile("movl %%esi, %%eax": "=a" (esi) : );
    asm volatile("movl %%edi, %%eax": "=a" (edi) : );
    asm volatile("movl %%esp, %%eax": "=a" (esp) : );
    asm volatile("movl %%ebp, %%eax": "=a" (ebp) : );
    terminal_writestring("CS=");
    hex_to_ascii(cs, 4, str);
    terminal_writestring(str);
    terminal_writestring(" DS=");
    hex_to_ascii(ds, 4, str);
    terminal_writestring(str);
    terminal_writestring(" ES=");
    hex_to_ascii(es, 4, str);
    terminal_writestring(str);
    terminal_writestring(" FS=");
    hex_to_ascii(fs, 4, str);
    terminal_writestring(str);
    terminal_writestring(" GS=");
    hex_to_ascii(gs, 4, str);
    terminal_writestring(str);
    terminal_newline();
    terminal_writestring("    ");
    terminal_writestring("EAX=");
    hex_to_ascii(eax, 8, str);
    terminal_writestring(str);
    terminal_writestring(" EBX=");
    hex_to_ascii(ebx, 8, str);
    terminal_writestring(str);
    terminal_writestring(" ECX=");
    hex_to_ascii(ecx, 8, str);
    terminal_writestring(str);
    terminal_writestring(" EDX=");
    hex_to_ascii(edx, 8, str);
    terminal_writestring(str);
    terminal_newline();
    terminal_writestring("    ");
    terminal_writestring("ESI=");
    hex_to_ascii(esi, 8, str);
    terminal_writestring(str);
    terminal_writestring(" EDI=");
    hex_to_ascii(edi, 8, str);
    terminal_writestring(str);
    terminal_writestring(" ESP=");
    hex_to_ascii(esp, 8, str);
    terminal_writestring(str);
    terminal_writestring(" EBP=");
    hex_to_ascii(ebp, 8, str);
    terminal_writestring(str);
}