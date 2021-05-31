; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; Multiboot header
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

; Set up the stack
section .bss
align 16
stack_bottom:
resb 16384
stack_top:

; Kernel entrypoint
section .text
global _start:function (_start.end - _start)
_start:
    mov esp, stack_top

    ; Call global constructors
    extern _init
    call _init

    ; Transfer control to kernel
    extern kernel_main
    call kernel_main

    ; Hang if we somehow get here
    cli
    hlt
    jmp $
.end
