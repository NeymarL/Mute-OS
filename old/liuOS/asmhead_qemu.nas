; haribote-os boot asm
; TAB=4

BOTPAK  EQU     0x00280000      ; 我们主程序load地址
DSKCAC  EQU     0x00100000      ; ディスクキャッシュの場所
DSKCAC0 EQU     0x00008000      ; ディスクキャッシュの場所（リアルモード）

; BOOT_INFO関係
CYLS    EQU     0x0ff0          ; 柱面数
LEDS    EQU     0x0ff1
VMODE   EQU     0x0ff2          ; 色数に関する情報。何ビットカラーか？
SCRNX   EQU     0x0ff4          ; 解像度のX
SCRNY   EQU     0x0ff6          ; 解像度のY
VRAM    EQU     0x0ff8          ; グラフィックバッファの開始番地

        ORG     0x8200          ; 程序开始的地址

; 画面モードを設定

        MOV     AL,0x13         ; VGAグラフィックス、320x200x8bitカラー
        MOV     AH,0x00
        INT     0x10
        MOV     BYTE [VMODE],8  ; 画面モードをメモする（C言語が参照する）
        MOV     WORD [SCRNX],320
        MOV     WORD [SCRNY],200
        MOV     DWORD [VRAM],0x000a0000

; キーボードのLED状態をBIOSに教えてもらう

        MOV     AH,0x02
        INT     0x16            ; keyboard BIOS
        MOV     [LEDS],AL

; PICが一切の割り込みを受け付けないようにする
;   AT互換機の仕様では、PICの初期化をするなら、
;   こいつをCLI前にやっておかないと、たまにハングアップする
;   PICの初期化はあとでやる
; 禁用pic中断

        MOV     AL,0xff
        OUT     0x21,AL
        NOP                     ; OUT命令を連続させるとうまくいかない機種があるらしいので
        OUT     0xa1,AL

        CLI                     ; 禁用可屏蔽中断

; 开启A20管脚

        CALL    waitkbdout
        MOV     AL,0xd1
        OUT     0x64,AL
        CALL    waitkbdout
        MOV     AL,0xdf         ; enable A20
        OUT     0x60,AL
        CALL    waitkbdout

; プロテクトモード移行

;[INSTRSET "i486p"]              ; 486の命令まで使いたいという記述

        LGDT    [GDTR0]         ; 暫定GDTを設定
        MOV     EAX,CR0
        AND     EAX,0x7fffffff  ; 禁止分页
        OR      EAX,0x00000001  ; 开启保护模式
        MOV     CR0,EAX         ; protected mode
        JMP     pipelineflush   ; 段偏移量 
pipelineflush:
        MOV     AX,1*8          ;  読み書き可能セグメント32bit
        MOV     DS,AX
        MOV     ES,AX
        MOV     FS,AX
        MOV     GS,AX
        MOV     SS,AX

; bootpackの転送

        MOV     ESI,bootpack    ; 把我们的主程序加载到0x280000
        MOV     EDI,BOTPAK      ; 転送先
        MOV     ECX,512*1024/4
        CALL    memcpy

; ついでにディスクデータも本来の位置へ転送

; まずはブートセクタから

        MOV     ESI,0x7c00      ; 把boot程序加载到DSKCAC
        MOV     EDI,DSKCAC      ; 転送先
        MOV     ECX,512/4
        CALL    memcpy

; 残り全部

        MOV     ESI,DSKCAC0+512 ; 把其余的也加载进来
        MOV     EDI,DSKCAC+512  ; 転送先
        MOV     ECX,0
        MOV     CL,BYTE [CYLS]
        IMUL    ECX,512*18*2/4  ; シリンダ数からバイト数/4に変換
        SUB     ECX,512/4       ; IPLの分だけ差し引く
        CALL    memcpy

; asmheadでしなければいけないことは全部し終わったので、
;   あとはbootpackに任せる

; bootpackの起動

        ;MOV     EBX,BOTPAK
        ;MOV     ECX,[EBX+16]
        ;ADD     ECX,3           ; ECX += 3;
        ;SHR     ECX,2           ; ECX /= 4;
        ;JZ      skip            ; 転送するべきものがない
        ;MOV     ESI,[EBX+20]    ; 転送元
        ;ADD     ESI,EBX
        ;MOV     EDI,[EBX+12]    ; 転送先
        ;CALL    memcpy
;skip:
        ;MOV     ESP,[EBX+12]    ; スタック初期値
        ;JMP     DWORD 2*8:0x0000001b

        MOV     ESP, BOTPAK
        JMP     DWORD 16:0x00000000


waitkbdout:
        IN       AL,0x64
        AND      AL,0x02
        JNZ     waitkbdout      ; ANDの結果が0でなければwaitkbdoutへ
        RET

memcpy:
        ;MOV     EAX,[ESI]
        ;ADD     ESI,4
        ;MOV     [EDI],EAX
        ;ADD     EDI,4
        ;SUB     ECX,1
        ;JNZ     memcpy          ; 引き算した結果が0でなければmemcpyへ
        CLD
        A32     REP     MOVSD
        RET
; memcpyはアドレスサイズプリフィクスを入れ忘れなければ、ストリング命令でも書ける

        ALIGNB  16
GDT0:
        RESB    8               ; ヌルセレクタ
        DW      0xffff,0x0000,0x9200,0x00cf ; 読み書き可能セグメント32bit
        DW      0xffff,0x0000,0x9a28,0x0047 ; 実行可能セグメント32bit（bootpack用）

        DW      0
GDTR0:
        DW      8*3-1
        DD      GDT0

        ALIGNB  16
bootpack:
