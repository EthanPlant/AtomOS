; GDT
gdt_start:

; Mandatory null descriptor
gdt_null:
    dd 0x0
    dd 0x0

; Code segment descriptor
gdt_code:
    dw 0xffff ; Limit (bits 0-15)
    dw 0x0 ; Base (bits 0-15)
    db 0x0 ; Base (bits 16-23)
    db 10011010b ; Type flags
    db 11001111b ; Other flags + limit (bits 16-19)
    db 0x0 ; Base (bits 24-31)

gdt_data:
    dw 0xffff ; Limit (bits 0-15)
    dw 0x0 ; Base (bits 0-15)
    db 0x0 ; Base (bits 16-23)
    db 10010010b ; Type flags
    db 11001111b ; Other flags + limit (bits 16-19)
    db 0x0 ; Base (bits 24-31)

gdt_end: ; Used to calculate GDT size

; GDT Descriptor
gdt_descriptor:
    dw gdt_end-gdt_start-1 ; Size of the GDT
    dd gdt_start ; Start address of the GDT

; Define some constants
CODE_SEG equ gdt_code-gdt_start
DATA_SEG equ gdt_data-gdt_start
