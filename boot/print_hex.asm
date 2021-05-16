print_hex:
    pusha
    mov cx, 0

hex_loop:
    cmp cx, 4 ; Loop 4 times
    je end

    ; Convert char to ASCII
    mov ax, dx
    and ax, 0x000f ; Mask the first 3 zeros
    add al, 0x30 ; Add 0x30 to char to convert it to ASCII
    cmp al, 0x39 ; if > 9, add to represent ASCII 'A' through 'F'
    jle step2
    add al, 7

step2:
    ; Get correct position to place ASCII char
    mov bx, HEX_OUT+5 ; String location + length
    sub bx, cx
    mov [bx], al
    ror dx, 4 ; Rotate 4 times

    add cx, 1
    jmp hex_loop

end:
    mov bx, HEX_OUT
    call print_string

    popa
    ret

HEX_OUT:
    db '0x0000',0