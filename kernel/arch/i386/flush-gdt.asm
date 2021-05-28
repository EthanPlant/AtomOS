global gdt_flush ; Allows C to call this
gdt_flush:
    mov eax, [esp+4] ; Get the GDT pointer passed from C
    lgdt [eax] ; Load the GDT

    ; Update the data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to update cs register
    jmp 0x08:flush
flush: ret