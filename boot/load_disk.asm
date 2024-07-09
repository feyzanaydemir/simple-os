load_disk:
    mov ah, 0x02        ; int 0x13 function (0x02 = 'read')
    mov al, 0x36        ; Number of sectors to read (0x01 ... 0x80)
    mov cl, 0x02        ; Sector (0x01 ... 0x11)
    mov ch, 0x00        ; Cylinder (0x0 ... 0x3ff)
    mov dl, [DRIVE_NUM] ; Drive number (0 = floppy, 0x80 = hdd)
    mov dh, 0x00        ; Head number (0x0 ... 0xf)
    
    int 0x13
    jc disk_error       ; Error if the carry bit is set
    ret

disk_error:
    mov si, MSG_DISK_ERROR
    call rm_print
    jmp $

;------------------------------------

MSG_DISK_ERROR db 'Disk read error.', 13, 10, 0
