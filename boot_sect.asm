; The most basic boot sector possible

loop:
    jmp loop ; Set up an infinite loop

; Pad with 0s
times 510-($-$$) db 0

dw 0xaa55 ; Magic number