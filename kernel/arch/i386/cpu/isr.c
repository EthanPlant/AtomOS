#include <stdint.h>

#include <cpu/idt.h>
#include <cpu/isr.h>
#include <drivers/ports.h>
#include <drivers/video/vga.h>
#include <kernel/tty.h>
#include <kernel/utils.h>


isr_t interrupt_handlers[256];

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

// Fault handlers
void doublefault_handler(registers_t *regs)
{
    panic(__FILE__, "CPU ERROR: Double Fault detected!", __LINE__);
}

void gpfault_handler(registers_t *regs)
{
    panic(__FILE__, "CPU ERROR: General Protection Fault detected!", __LINE__);
}


void pagefault_handler(registers_t *regs)
{
    panic(__FILE__, "CPU ERROR: Page Fault detected!", __LINE__);
}

void isr_install(void)
{
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("Installing ISRs and enabling interrupts... ");

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

    register_interrupt_handler(0x08, doublefault_handler);
    register_interrupt_handler(0x0D, gpfault_handler);
    register_interrupt_handler(0x0E, pagefault_handler);

    // Remap the PIC
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Init master PIC
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); // Init slave PIC
    outb(PIC1_DATA, 0x20); // Master PIC offset
    outb(PIC2_DATA, 0x28); // Slave PIC offset
    outb(PIC1_DATA, 0x04); // Tell master PIC there's a slave at IRQ2
    outb(PIC2_DATA, 0x02); // Tell slave PIC its ID
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);

    set_idt_gate(IRQ0, (uint32_t)irq0);
    set_idt_gate(IRQ1, (uint32_t)irq1);
    set_idt_gate(IRQ2, (uint32_t)irq2);
    set_idt_gate(IRQ3, (uint32_t)irq3);
    set_idt_gate(IRQ4, (uint32_t)irq4);
    set_idt_gate(IRQ5, (uint32_t)irq5);
    set_idt_gate(IRQ6, (uint32_t)irq6);
    set_idt_gate(IRQ7, (uint32_t)irq7);
    set_idt_gate(IRQ8, (uint32_t)irq8);
    set_idt_gate(IRQ9, (uint32_t)irq9);
    set_idt_gate(IRQ10, (uint32_t)irq10);
    set_idt_gate(IRQ11, (uint32_t)irq11);
    set_idt_gate(IRQ12, (uint32_t)irq12);
    set_idt_gate(IRQ13, (uint32_t)irq13);
    set_idt_gate(IRQ14, (uint32_t)irq14);
    set_idt_gate(IRQ15, (uint32_t)irq15);

    asm volatile("sti");

    terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("done\n");
}

void isr_handler(registers_t *regs)
{
    // Handle interrupt
    if (interrupt_handlers[regs->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }

    char *int_no_str = "";
    char *int_err_code_str = "";
    terminal_writestring("Recieved interrupt: ");
    itoa(regs->int_no, int_no_str, 16);
    terminal_writestring(int_no_str);
    terminal_writestring("\nError message: ");
    terminal_writestring(exception_messages[regs->int_no]);
    terminal_writestring("\nError Code: ");
    itoa(regs->err_code, int_err_code_str, 2);
    terminal_writestring(int_err_code_str);
    terminal_newline();
}

void irq_handler(registers_t *regs)
{
    // Need to send an EOI to the PICS
    if (regs->int_no >= 40) outb(PIC2_COMMAND, 0x20); // Slave
    outb(PIC1_COMMAND, 0x20); // Master

    // Handle interrupt
    if (interrupt_handlers[regs->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }

}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}