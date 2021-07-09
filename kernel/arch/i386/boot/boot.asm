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

section .lowerhalf ; Used to tell linker this should be placed in lower half of mem

; Multiboot header
align 4
boot_header:
dd MAGIC
dd FLAGS
dd CHECKSUM

; Set up temporary page table for higher-half
; Later changed by our virtual memory manager
global TMP_PG_DIR
align 4096 ; Align to page boundaries
TMP_PG_DIR:
    ; Identity map the first 4 MB
    ; Needed to avoid triple faulting on paging enable
    ; Later this will be unmapped by our virtual memory manager
    dd 0x00000083 ; Page table entry: kernel, write enabled, present, mapped to address 0, but 7 is set to tell the CPU this is a 4 MB page
    times (PDE_INDEX - 1) dd 0 ; Fill with 0s until the next entry
    dd 0x00000083 ; Same as above, maps 0xC0000000 to physical address 0
    times (1024 - PDE_INDEX - 1) dd 0 ; Fill up the rest of the table with 0

align 4
extern kernel_main
global _start
_start:
    ; Update page directory
    ; ecx is our working register as eax and ebx are used by multiboo
    mov ecx, TMP_PG_DIR
    mov cr3, ecx

    ; Enable 4 MB pages, gets changed later but simplifies our life right now
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

    ; Pass control over to the kernel
    call kernel_main

    ; Hang if we somehow get here
    cli
    hlt 
    jmp $