; A simple boot sector 

[org 0x7c00] ; Offset our code

mov [BOOT_DRIVE], dl ; Store the boot drive

; Initialize the stack
mov bp, 0x8000
mov sp, bp

; Load 5 sectors from the disk
mov bx, 0x9000
mov dh, 5
mov dl, [BOOT_DRIVE]
call disk_load

mov dx, [0x9000] ; Print the first loaded word
call print_hex

call print_nl

mov dx, [0x9000 + 512] ; Print the first word from the second loaded sector
call print_hex

jmp $ ; Hang

%include "print_string.asm"
%include "print_hex.asm"
%include "disk_load.asm"

; Data
BOOT_DRIVE:
    db 0

; Padding and magic number
times 510-($-$$) db 0
dw 0xaa55

; Add some dummy data beyond the boot sector
times 256 dw 0xdada
times 256 dw 0xface