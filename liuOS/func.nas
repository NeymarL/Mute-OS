; func
; TAB=4

;[FORMAT "WCOFF"]        ; 制作32位目标文件格式
;[BITS 32]               ; 制作32位模式用的机器语言

; 制作目标文件的信息

;[FILE "func.nas"]       ; 源文件名信息

        GLOBAL      _io_hlt     ; 程序中包涵的函数名

; 以下是实际函数

_io_hlt:                ; void io_hlt(void);
        HLT 
        RET

