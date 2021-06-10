; Multiboot constats
; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

STACK_SIZE equ 16384

VM_BASE equ 0xC0000000
PDE_INDEX equ (VM_BASE >> 22)
PSE_BIT equ 0x00000010
PG_BIT equ 0x80000000

bits 32

; Reserve stack size
section .bss
align 4
stack_bottom:
resb STACK_SIZE
stack_top:

section .lowerhalf
; Multiboot header
align 4
boot_header:
dd MAGIC
dd FLAGS
dd CHECKSUM

global TMP_PG_DIR
align 4096 ; Align to page boundaries
TMP_PG_DIR:
    ; Identity map the first 4 MB temporarily. Needed to avoid triple faulting
    dd 0x00000083
    times (PDE_INDEX - 1) dd 0
    dd 0x00000083
    times (1024 - PDE_INDEX - 1) dd 0

align 4
extern kernel_main
global _start
_start:
    ; Update page directory. Need to use ecx as eax and ebx are in use by multiboot
    mov ecx, TMP_PG_DIR
    mov cr3, ecx

    ; Enable 4 MB pages
    mov ecx, cr4
    or ecx, PSE_BIT
    mov cr4, ecx

    ; Enable paging
    mov ecx, cr0
    or ecx, PG_BIT
    mov cr0, ecx

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