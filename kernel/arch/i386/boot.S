# Declare multiboot constants
.set ALIGN, 1 << 0
.set MEMINFO, 1 << 1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Declare multiboot header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16K
stack_top:

# Kernel entrypoint
.section .text
.global _start
.type _start, @function
_start:
    movl $stack_top, %esp

    # Call global constructors
    call _init

    # Transfer control to the kernel
    call kernel_main

    # Hang if we unexepectedly return
    cli
1:  hlt
    jmp 1b

.size _start, . - _start 
