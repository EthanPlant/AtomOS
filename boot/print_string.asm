; Print a string
print_string:
    pusha ; Save contents of all registers

start:
    mov al, [bx] ; Move current char to al
    cmp al, 0 ; Check if end of string
    je done

    ; Print this char
    mov ah, 0x0e
    int 0x10

    ; Loop
    add bx, 1
    jmp start

done:
    popa
    ret

; Print a newline
print_nl:
    pusha

    mov ah, 0x0e
    mov al, 0x0a ; Newline character
    int 0x10
    mov al, 0x0d ; Carraige return
    int 0x10

    popa
    ret