; A simple boot sector 

[org 0x7c00] ; Offset our code

; Initialize the stack
mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call switch_to_pm ; Note we never return from here

jmp $ ; Hang if we somehow do

%include "print_string.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_pm.asm"

[bits 32]
; Where we arrive after switching to protected mode
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    jmp $ ; hang

; Global variables
MSG_REAL_MODE db "Started in 16-bit real mode",0
MSG_PROT_MODE db "Sucessfully switched to 32-bit protected mode",0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55