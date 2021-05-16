[bits 32]
; Define some contants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; print a null-terminated string pointed to by edx
print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY ; Set edx to start of video memory

print_string_pm_loop:
    mov al, [ebx] ; Store the character at ebx in al
    mov ah, WHITE_ON_BLACK ; Store the attributes in ah

    ; Check if at end of string
    cmp al, 0
    je print_string_pm_done

    mov [edx], ax ; Store char and attributes
    add ebx, 1 ; Increment to next character
    add edx, 2 ; Move to next cell in video memory

    jmp print_string_pm_loop ; Loop until done

print_string_pm_done:
    popa
    ret