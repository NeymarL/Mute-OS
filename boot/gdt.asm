; boot loader
; Jump to protect mode & Load the kernel

%include        "pm.inc"

org     0x7c00

        jmp     LABEL_BEGIN

[SECTION .gdt]
; GDT                                段基址,           段界限,         属性
LABEL_GDT :             Descriptor       0,                0,            0       ; 空描述符
LABEL_DESC_CODE32 :     Descriptor       0, SegCode32Len - 1, DA_C + DA_32       ; 非一致代码段
LABEL_DESC_VIDEO :      Descriptor 0B8000h,           0FFFFh,       DA_DRW       ; 显存首地址
; GDT结束

GdtLen          equ             $ - LABEL_GDT           ; GDT长度
GdtPtr          dw              GdtLen - 1              ; GDT界限
                dd              0                       ; GDT基址

; GDT选择子
SelectorCode32  equ             LABEL_DESC_CODE32 - LABEL_GDT
SelectorVideo   equ             LABEL_DESC_VIDEO  - LABEL_GDT
; end of [SECTION .gdt]

[SECTION .s16]
[BITS 16]
LABEL_BEGIN :
        mov     ax, cs
        mov     ds, ax
        mov     es, ax
        mov     sp, 0100h

        ; 初始化32位代码段描述符
        xor     eax, eax
        mov     ax, cs
        shl     eax, 4
        add     eax, LABEL_DESC_CODE32
        mov     word [LABEL_DESC_CODE32 + 2], ax
        shr     eax, 16
        mov     byte [LABEL_DESC_CODE32 + 4], al
        mov     byte [LABEL_DESC_CODE32 + 7], ah

        ; 为加载 GDTR 做准备
        xor     eax, eax
        mov     ax, ds
        shl     eax, 4
        add     eax, LABEL_GDT          ; eax <- gdt基址
        mov     dword [GdtPtr + 2], eax ; [GdtPtr + 2] <- gdt基址

        ; 加载 GDTR
        lgdt    [GdtPtr]

        ; 禁用中断
        cli

        ; 打开A20地址线
        in      al, 92h
        or      al, 00000010b
        out     92h, al

        ; 准备切换到保护模式
        mov     eax, cr0
        or      eax, 1
        mov     cr0, eax

        ; 进入保护模式
        ; 执行这一句话会把 SelectorCode32 装入 cs, 并跳转到 SelectorCode32:0 处
        jmp     dword   SelectorCode32:0        

; end of [SECTION .s16]

[SECTION .s32]
[BITS 32]

LABEL_SEG_CODE32:
        mov     ax, SelectorVideo
        mov     gs, ax                  ; 视频段选择子

        mov     edi, (80 * 11 + 79) * 2 ; 屏幕第11行, 第79列
        mov     ah, 0Ch                 ; 0000 : 黑底, 1100 : 红字
        mov     al, 'P'
        mov     [gs:edi], ax

        jmp     $

SegCode32Len    equ     $ - LABEL_SEG_CODE32

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
