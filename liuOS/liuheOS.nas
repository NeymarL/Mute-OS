; liuhe-OS
; TAB=4

; 有关BOOT_INFO

CYLS    EQU     0x0ff0
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2      ; 颜色的位数
SCRNX   EQU     0x0ff4      ; 分辨率的X
SCRNY   EQU     0x0ff6      ; 分辨率的Y
VRAM    EQU     0x0ff8      ; 图像缓存区的开始地址

        ORG     0x8200

        ;MOV     AX, 0x4F02
        MOV     AL, 0x13    ; VGA显卡,320*200*8
        MOV     AH, 0x00
        INT     0x10
        MOV     BYTE [VMODE], 8 ; 记忆画面模式
        MOV     WORD [SCRNX], 320
        MOV     WORD [SCRNY], 200
        MOV     DWORD [VRAM], 0x000a0000

; 用BIOS取得键盘上个种指示灯的状态

        MOV     AH, 0x02
        INT     0x16
        MOV     [LEDS], AL


fin:
        HLT
        JMP     fin