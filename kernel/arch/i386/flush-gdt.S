.global gdt_flush
.text
gdt_flush:
    movl 4(%esp), %eax # Get the gdt pointer passed as a parameters
    lgdt (%eax) # Load the GDT

    # Update data registers
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Far jump to update code segment
    ljmp $0x08, $flush

flush:
    ret 