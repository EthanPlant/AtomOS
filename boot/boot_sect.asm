; A simple boot sector 

[org 0x7c00] ; Offset our code
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl ; BIOS stores the boot drive in dl

; Initialize the stack
mov bp, 0x9000
mov sp, bp

; Announce we're starting
mov bx, MSG_REAL_MODE
call print_string

; Load the kernel
call load_kernel

; Switch to protected mode
call switch_to_pm

jmp $

; Includes
%include "boot/print_string.asm"
%include "boot/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/print_string_pm.asm"
%include "boot/switch_to_pm.asm"

[bits 16]

load_kernel:
    ; Announce we're loading the kernel into memory
    mov bx, MSG_LOAD_KERNEL
    call print_string

    ; Set up params for our disk load routine
    mov bx, KERNEL_OFFSET 
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    call disk_load

    ret

; Where we arrive after switching to protected mode
[bits 32]
BEGIN_PM:
    ; Announce we're in 32-bit mode
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    call KERNEL_OFFSET ; Call the kernel entrypoint

    jmp $ ; Hang

; Global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit real mode",0
MSG_PROT_MODE db "Succesfully switched to 32-bit protected mode",0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55