#include <stdint.h>
#include <string.h>

#include <idt.h>

extern void idt_flush(uint32_t);


idt_entry_t idt_entries[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void set_idt_gate(int n, uint32_t handler)
{
    idt_entries[n].base_low = handler & 0xFFFF;
    idt_entries[n].base_high = (handler >> 16) & 0xFFFF;

    idt_entries[n].sel = KERNEL_CS;
    idt_entries[n].always0 = 0;
    idt_entries[n].flags = 0x8E;
}

void init_idt()
{
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_ENTRIES) - 1;
    idt_ptr.base = (uint32_t) &idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    idt_flush((uint32_t*)&idt_ptr);
}