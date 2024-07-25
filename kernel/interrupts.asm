[extern isr_handler]

;------------------------------------
%macro isr_stub 1
isr%+%1:
    push byte 0
    push byte %1
    jmp int_service_1
%endmacro

isr_stub 0
isr_stub 1
isr_stub 2
isr_stub 3
isr_stub 4
isr_stub 5
isr_stub 6
isr_stub 7
isr_stub 8
isr_stub 9
isr_stub 10
isr_stub 11
isr_stub 12
isr_stub 13
isr_stub 14
isr_stub 15
isr_stub 16
isr_stub 17
isr_stub 18
isr_stub 19
isr_stub 20
isr_stub 21
isr_stub 22
isr_stub 23
isr_stub 24
isr_stub 25
isr_stub 26
isr_stub 27
isr_stub 28
isr_stub 29
isr_stub 30
isr_stub 31

global isr_stub_table
isr_stub_table:
%assign i 0 
%rep 32 
    dd isr%+i
%assign i i+1 
%endrep

;------------------------------------
%macro irq_stub 2
irq%+%1:
    push byte %1
    push byte %2
    jmp int_service_2 
%endmacro

irq_stub 0, 32
irq_stub 1, 33
irq_stub 2, 34
irq_stub 3, 35
irq_stub 4, 36
irq_stub 5, 37
irq_stub 6, 38
irq_stub 7, 39
irq_stub 8, 40
irq_stub 9, 41
irq_stub 10, 42
irq_stub 11, 43
irq_stub 12, 44
irq_stub 13, 45
irq_stub 14, 46
irq_stub 15, 47

global irq_stub_table
irq_stub_table:
%assign i 0 
%rep 16 
    dd irq%+i
%assign i i+1 
%endrep

;------------------------------------
%macro int_service_stub 3
int_service_%+%1:
    pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    cld
    call isr_handler
    pop %2
    pop %2
    mov ds, %3
    mov es, %3
    mov fs, %3
    mov gs, %3
    popa
    add esp, 8
    iret
%endmacro

int_service_stub 1, eax, ax
int_service_stub 2, ebx, bx
