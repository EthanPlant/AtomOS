#include <isr.h>
#include <idt.h>
#include <stdint.h>
#include <stdio.h>

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


void isr_install(void)
{
    // Install the ISRs for the first 31 interrupts used by the CPU 
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    // TODO IRQ stuff
}

void isr_handler(registers_t *regs)
{
    char* str;
    terminal_writestring("Recieved interrupt: ");
    hex_to_ascii(regs->int_no, str);
    terminal_writestring(str);
    terminal_writestring("\nError message: ");
    terminal_writestring(exception_messages[regs->int_no]);
    terminal_writestring("\nError Code: ");
    hex_to_ascii(regs->err_code, str);
    terminal_writestring(str);
    terminal_writestring("\n");
}

// Helper function because we can't print numbers rn
void hex_to_ascii(int32_t n, char *str)
{
    int32_t tmp;
    int index = 2;
    str[0] = '0';
    str[1] = 'x';
    for (int i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp > 0xA) str[index++] = tmp - 0xA + 'a';
        else str[index++] = tmp + '0';
    } 
    tmp = n & 0xF;
    if (tmp > 0xA) str[index++] = tmp - 0xA + 'a';
    else str[index++] = tmp + '0';
    str[index] = 0; // Null terminate the string
}