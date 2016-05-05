; ipl
; TAB=4

CYL     EQU     10
CYLS    EQU     0x0ff0
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2      ; 颜色的位数
SCRNX   EQU     0x0ff4      ; 分辨率的X
SCRNY   EQU     0x0ff6      ; 分辨率的Y
VRAM    EQU     0x0ff8      ; 图像缓存区的开始地址

start:  ORG     0x7c00      ; 指明程序装载地址

; 标准FAT12格式软盘专用代码

        JMP     entry
        DB      0x90
        DB      "HELLOIPL"  ; 启动区的名称可以是任意字符串
        DW      512         ; 每个扇区(sector)的大小，必须为512
        DB      1           ; 簇(cluster)的大小，必须为1个扇区
        DW      1           ; FAT的起始位置(一般从第一个扇区开始)
        DB      2           ; FAT的个数，必须为2
        DW      224         ; 根目录大小，一般设成224项
        DW      2880        ; 该磁盘的大小，必须是2880扇区
        DB      0xf0        ; 磁盘的种类，必须是0xf0
        DW      9           ; FAT的长度，必须是9扇区
        DW      18          ; 1个磁道(track)有几个扇区，必须是18
        DW      2           ; 磁头数，必须是2
        DD      0           ; 不使用分区，必须是0
        DD      2880        ; 重写一次磁盘大小
        DB      0, 0, 0x29      ; 意义不明，固定
        DD      0xffffffff      ; 可能是卷标号码
        DB      "Mute-OS    "   ; 磁盘的名称，11字节
        DB      "FAT12   "      ; 磁盘个是名称，8字节
        RESB    18             ; 先空出18字节

; 程序主体

entry:

        MOV     AX, 0       ; 初始化寄存器
        MOV     SS, AX
        MOV     SP, 0x7c00
        MOV     DS, AX

        MOV     AX, 0x0820
        MOV     ES, AX
        MOV     CH, 0       ; 柱面0
        MOV     DH, 0       ; 磁头0
        MOV     CL, 2       ; 扇区2

readloop:
        MOV     SI, 0       ; 记录失败次数

retry:

        MOV     AH, 0x02    ; AH=0x02 : 读盘
        MOV     AL, 1       ; 1个扇区
        MOV     BX, 0
        
        ;MOV     DL, 0x80    ; USB驱动器
        MOV     DL, 0x00     ; IN QEMU!!!

        INT     0x13        ; 调用磁盘BIOS
        JNC     next        ; 没出错时跳转到next
        ADD     SI, 1       ; SI加一
        CMP     SI, 5       ; 比较SI与5
        JAE     error       ; jump if above or equal
        MOV     AH, 0x00    ;
        
        ;MOV     DL, 0x80    ; USB驱动器
        MOV     DL, 0x00     ; IN QEMU!!!

        INT     0x13        ; 重置驱动器
        JMP     retry
next:
        MOV     AX, ES      ; 把内存地址向后移0x200
        ADD     AX, 0x0020  ; 通过AX加上0x0020, 相当于将地址后移0x200
        MOV     ES, AX      ; 因为没有 ADD ES, 0x020指令
        ADD     CL, 1       ; sector++
        
        ;CMP     CL, 63      ; cl与63比较
        CMP     CL, 18      ; in QEMU !!!

        JBE     readloop    ; jump if below or equal

        MOV     CL, 1
        ADD     DH, 1       ; header++
        
        ;CMP     DH, 10
        CMP     DH, 2       ; in QEMU !!!

        JB      readloop

        MOV     DH, 0
        ADD     CH, 1       ; cylinders++
        CMP     CH, CYL
        JBE     readloop

        ;JMP     sayhello
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

        JMP     0x8200

fin:
        HLT                 ; 让CPU停止，等待指令
        JMP     fin         ; 无限循环

sayhello:
        MOV     SI, hello
        JMP     putloop

error:
        MOV     SI, errmsg

putloop:
        MOV     AL, [SI]
        ADD     SI, 1       ; 给SI加1
        CMP     AL, 0

        JE      fin
        MOV     AH, 0x0e    ; 显示一个文字
        MOV     BX, 15      ; 指定字符颜色
        INT     0x10        ; 调用显卡BIOS
        JMP     putloop


errmsg:
        DB      0x0a, 0x0a  ; 换行两次
        DB      "Load ERROR!"
        DB      0x0a
        DB      0

hello:
        DB      0x0a, 0x0a  ; 换行两次
        DB      "Welcome to Mute-OS!"
        DB      0x0a
        DB      0


marker: RESB 0x1fe - (marker - start)  ; 填写0x00，直到0x001fe
        DB  0x55, 0xaa

