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

        push    rsi
        push    rdi
        push    rcx

        mov     rdi, rcx    ; Destination
        mov     rsi, rdx    ; Source
        mov     rcx, r8     ; Counter
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

        pop     rcx
        pop     rdi
        pop     rsi
        mov     rsp, rbp
        pop     rbp

        ret         ; 函数结束，返回
; memcpy 结束-------------------------------------------------------------

; ========================================================================
;                  void print(const char * pszinfo);
; ========================================================================
print:
        push    rbp
        mov     ebp, esp

        mov     rsi, rdi        ; pszInfo
        mov     edi, dword [disp_pos]
        mov     ah, 0Fh
.1:
        lodsb
        test    al, al
        jz      .2
        cmp     al, 0Ah ; 是回车吗?
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
        mov     [gs:edi], ax
        add     edi, 2
        jmp     .1

.2:
        mov     dword [disp_pos], edi

        pop     rbp
        ret
;----------------------------------------------------------------------------
