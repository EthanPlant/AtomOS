global idt_flush
idt_flush:
    mov eax, [esp+4] ; Get pointer from stack
    lidt [eax] ; Load IDT
    ret

; Macros
%macro ISR_NOERRCODE 1
    [global isr%1]
    isr%1:
        push byte 0
        push %1
        jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    [global isr%1]
    isr%1:
        push %1
        jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

[extern isr_handler]

isr_common_stub:
    pusha

    ; Save data segment
    mov ax, ds
    push eax

    ; Load kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp ; registers_t *regs

    call isr_handler

    pop eax

    ; Reload data segment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8 ; Clean up the pushes made to the stack
    iret

global int_test
int_test:
    int 0x03
    int 0x04
    ret