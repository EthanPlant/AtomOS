; A simple boot sector that prints "Hello" the the screen with BIOS interrupts

mov ah, 0x0e ; Set to tele-type mode

; Write "Hello"
mov al, 'H' ; Write H to al
int 0x10 ; Call the screen interrupt
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
int 0x10
mov al, 'o'
int 0x10

jmp $ ; infinite loop

; Padding and magic bootsect number
times 510-($-$$) db 0
dw 0xaa55