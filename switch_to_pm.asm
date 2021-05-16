[bits 16]
; Switch to protected mode
switch_to_pm:
    cli ; Disable interrupts
    lgdt[gdt_descriptor] ; Load the GDT

    ; Set processor to 32-bit protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Make a far jump to 32-bit code
    jmp CODE_SEG:init_pm

[bits 32]
; Initialize registers and stack
init_pm:
    ; Don't need our old segments anymore as they're useless in this mode
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Update the stack
    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM