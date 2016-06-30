        org     0x7c00
        mov     ax, cs
        mov     ds, ax
        mov     es, ax
        call    DispStr
        jmp     $

DispStr:
        mov     ax, BootMessage
        mov     bp, ax                  ; ES:BP = 串地址
        mov     cx, 20                  ; CX = 串长度
        mov     ax, 01301h              ; AH = 13h, AL = 01h
        mov     bx, 000ch               ; BH = 0, BL = 0Ch 黑底红字
        mov     dl, 0
        int     10h
        ret

BootMessage:    db      "Welcome to MuteOS !!"

        times   510-($-$$)      db      0  ;
        dw      0xaa55
