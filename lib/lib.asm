; lib.asm
;

[BITS 32]

extern  disp_pos

[SECTION .text]

; 导出函数
global  memocpy
global  print
global  out_byte
global  in_byte
global  init_8259A_asm
global  DispInt


; ------------------------------------------------------------------------
; void* memocpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
memocpy:
        push    ebp
        mov     ebp, esp

        push    esi
        push    edi
        push    ecx

        mov     edi, [ebp + 8]  ; Destination
        mov     esi, [ebp + 12] ; Source
        mov     ecx, [ebp + 16] ; Counter
.1:
        cmp     ecx, 0          ; 判断计数器
        jz      .2              ; 计数器为零时跳出

        mov     al, [ds:esi]        ; ┓
        inc     esi                 ; ┃
                                    ; ┣ 逐字节移动
        mov     byte [es:edi], al   ; ┃
        inc     edi                 ; ┛

        dec     ecx     ; 计数器减一
        jmp     .1      ; 循环
.2:
        mov     eax, [ebp + 8]  ; 返回值

        pop     ecx
        pop     edi
        pop     esi
        mov     esp, ebp
        pop     ebp

        ret         ; 函数结束，返回
; memcpy 结束-------------------------------------------------------------

; ========================================================================
;       PUBLIC void  print(const char* pszInfo, const int color);
; ========================================================================
print:
        push    ebp
        mov     ebp, esp

        mov     esi, [ebp + 8]  ; pszInfo
        mov     ah, [ebp + 12]  ; color
        mov     edi, dword [disp_pos]
.1:
        lodsb
        test    al, al
        jz      .2
        cmp     al, 0Ah         ; 是回车吗?
        jnz     .3
        push    eax
        mov     eax, edi
        mov     bl, 160
        div     bl
        and     eax, 0FFh
        inc     eax
        mov     bl, 160
        mul     bl
        mov     edi, eax
        pop     eax
        jmp     .1
.3:
        mov     [gs:edi], ax
        add     edi, 2
        jmp     .1

.2:
        mov     dword [disp_pos], edi

        pop     ebp
        ret
;----------------------------------------------------------------------------

; ========================================================================
;                  void out_byte(u16 port, u8 value);
; ========================================================================
out_byte:
        mov     edx, [esp + 4]      ; port
        mov     al, [esp + 4 + 4]   ; value
        out     dx, al
        nop             ; 一点延迟
        nop
        ret

; ========================================================================
;                  u8 in_byte(u16 port);
; ========================================================================
in_byte:
        mov     edx, [esp + 4]      ; port
        xor     eax, eax
        in      al, dx
        nop                         ; 一点延迟
        nop
        ret
; ========================================================================


