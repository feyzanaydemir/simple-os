[bits 32]
pm_print:
    mov edi, 0xb8000

pm_print_loop:
    lodsb
    or al, al
    jz pm_print_done    ; End of string

    mov ah, 0x0f        ; White on black
    stosw
    jmp pm_print_loop

pm_print_done:
   ret
