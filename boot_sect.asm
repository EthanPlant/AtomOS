; A simple boot sector that prints a string

[org 0x7c00] ; Offset our code

; Print our strings
mov bx, HELLO_MSG
call print_string

call print_nl

mov bx, GOODBYE_MSG
call print_string

jmp $ ; Hang

%include "print_string.asm"

; Data
HELLO_MSG:
    db 'Hello, World',0
GOODBYE_MSG:
    db 'Goodbye',0

; Padding and magic number
times 510-($-$$) db 0
dw 0xaa55