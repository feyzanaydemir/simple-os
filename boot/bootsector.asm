[org 0x7c00]

global _start
_start:
    jmp main

;------------------------------------

%include "boot/rm_print.asm"
%include "boot/load_disk.asm"
%include "boot/gdt.asm"
%include "boot/pm_print.asm"

;------------------------------------

[bits 16]
main:
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x9000
    
    mov si, MSG_RM
    call rm_print

switch_to_pm:
    ; Read from disk and store in 0x1000 (kernel offset)
    mov bx, 0x1000
    call load_disk
    mov si, MSG_LOAD_KERNEL
    call rm_print

    cli
    ; Set 32-bit mode bit in cr0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Setup GDT
    lgdt [gdt_descriptor]
    mov ax, GDT_DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x9000
    jmp GDT_CODE_SEG:enter_pm

;------------------------------------

[bits 32]
enter_pm:
    mov esi, MSG_PM
    call pm_print
    call 0x1000
    jmp $

;------------------------------------

DRIVE_NUM db 0
MSG_RM db 'Booting...', 13, 10, 0
MSG_PM db 'Landed in 32-bit Protected Mode.', 0
MSG_LOAD_KERNEL db 'Loaded kernel into memory.', 13, 10, 0

; Padding and boot signature
times 510 - ($-$$) db 0
db 0x55
db 0xaa
