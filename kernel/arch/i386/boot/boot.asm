; Multiboot constats
; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

STACK_SIZE equ 16384

; Reserve stack size
section .bss
align 4
stack_bottom:
resb STACK_SIZE
stack_top:

; Multiboot header
section .multiboot
align 4
boot_header:
dd MAGIC
dd FLAGS
dd CHECKSUM

section .text
align 4
extern kernel_main
global _start
_start:
    ; Set up the stack
    mov esp, stack_top

    ; Push multiboot data to the stack for our kernel
    push eax
    push ebx

    ; Pass control over to the kerne
    call kernel_main

    ; Hang if we somehow get here
    cli
    hlt 
    jmp $