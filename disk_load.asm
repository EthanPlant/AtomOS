; Load sectors from the disk
disk_load:
    push dx ; Store dx on the stack so we can recall it later

    mov ah, 0x02 ; BIOS read function
    mov al, dh ; Read dh sectors
    mov ch, 0x00 ; Cylinder 0
    mov dh, 0x00 ; Head 0
    mov cl, 0x02 ; Start from the 2nd sector (right after the boot sector)
    int 0x13 ; BIOS interrupt

    jc disk_error ; Jump if error occured

    pop dx ; Restore dx from the stack
    cmp dh, al ; Check if we read the correct amount of sectors
    jne disk_error ; Display error message
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

; Data
DISK_ERROR_MSG:
    db "Disk read error!",0