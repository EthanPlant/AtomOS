#ifndef ARCH_I386_ISR_H
#define ARCH_I386_ISR_H

#include <stdint.h>

typedef struct registers
{
    uint32_t ds;
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, esp, ss;
} registers_t;

void isr_handler(registers_t *regs);

#endif