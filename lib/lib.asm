; lib.asm
;

[BITS 64]

[SECTION .data]
disp_pos    dd  0


[SECTION .text]

; 导出函数
global  memocpy
global  print

; ------------------------------------------------------------------------
; void* memocpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
memocpy:
        push    rbp
        mov     rbp, rsp

        mov     rdi, rdi    ; Destination
        mov     rsi, rsi    ; Source
        mov     rcx, rdx    ; Counter
.1:
        cmp     ecx, 0          ; 判断计数器
        jz      .2              ; 计数器为零时跳出

        mov     al, [ds:rsi]        ; ┓
        inc     esi                 ; ┃
                                    ; ┣ 逐字节移动
        mov     byte [es:rdi], al   ; ┃
        inc     edi                 ; ┛

        dec     ecx     ; 计数器减一
        jmp     .1      ; 循环
.2:
        mov     eax, [ebp + 8]  ; 返回值

        mov     rsp, rbp
        pop     rbp

        ret         ; 函数结束，返回
; memcpy 结束-------------------------------------------------------------

; ========================================================================
;       PUBLIC void  print(const char* pszInfo, const char color);
; ========================================================================
print:
        push    rbp
        push    rax
        mov     ebp, esp

        xor     rax, rax
        mov     eax, esi        ; color
        shl     ax, 8
        and     al, 0x0F
        mov     rsi, rdi        ; pszInfo
        mov     edi, dword [disp_pos]
.1:
        lodsb
        test    al, al
        jz      .2
        cmp     al, 0Ah         ; 是回车吗?
        jnz     .3
        push    rax
        mov     eax, edi
        mov     bl, 160
        div     bl
        and     eax, 0FFh
        inc     eax
        mov     bl, 160
        mul     bl
        mov     edi, eax
        pop     rax
        jmp     .1
.3:
        mov     [gs:rdi], ax
        add     edi, 2
        jmp     .1

.2:
        mov     dword [disp_pos], edi

        pop     rax
        pop     rbp
        ret
;----------------------------------------------------------------------------
