; 描述符
; usage : Descriptor Base, Limit, Attr
;         Base  :   dd
;         Limit :   dd (low 20 bits available)
;         Attr  :   dw (lower 4 bits of higher byte are always 0)

%macro Descriptor 3
        dw      %2 & 0x0FFFF                            ; 段界限1
        dw      %1 & 0x0FFFF                            ; 段基址1
        db      (%1 >> 16) & 0x0FF                      ; 段基址2
        dw      ((%2 >> 8) & 0x0F00) | (%3 & 0x0F0FF)   ; 属性1 + 段界限2 + 属性2
        db      (%1 >> 24) & 0x0FF                      ; 段基址3
%endmacro