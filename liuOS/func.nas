; func
; TAB=4

;[FORMAT "WCOFF"]        ; 制作32位目标文件格式
[BITS 32]               ; 制作32位模式用的机器语言

; 制作目标文件的信息

;[FILE "func.nas"]       ; 源文件名信息

        GLOBAL      io_hlt     ; 程序中包涵的函数名
        GLOBAL      test
        GLOBAL      write_mem8

; 以下是实际函数

io_hlt:                ; void io_hlt(voi
        HLT
        RET

test:
  MOV   EAX, 0xa0000
  JMP   loops
  RET

loops:
  MOV   BYTE [EAX],14
  INC   EAX
  CMP   EAX, 0xaffff
  JBE   loops
  RET

write_mem8:
    MOV     EAX, [ESP + 12]
    MOV     AL, [ESP + 41]
    MOV     [EAX], AL
    RET