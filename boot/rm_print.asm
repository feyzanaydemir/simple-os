rm_print:
   lodsb
   or al, al
   jz rm_print_done  ; End of string

   mov ah, 0x0e      ; Teletype output
   int 0x10
   jmp rm_print

rm_print_nl:
   mov ah, 0x0e
   mov al, 0x0d      ; Carriage return
   int 0x10
   
   mov al, 0x0a      ; Newline character
   int 0x10
   jmp rm_print_done

rm_print_done:
   ret
