; lib.asm
;

%include "sconst.inc"

[BITS 32]

extern  disp_pos

[SECTION .text]

; 导出函数
global  memocpy
global  print
global  out_byte
global  in_byte
global  memoset
global  strcpy
global  enable_irq
global  disable_irq
global  enable_int
global  disable_int


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
; memocpy 结束-------------------------------------------------------------

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

; ------------------------------------------------------------------------
; void memoset(void* p_dst, char ch, int size);
; ------------------------------------------------------------------------
memoset:
    push    ebp
    mov     ebp, esp

    push    esi
    push    edi
    push    ecx

    mov     edi, [ebp + 8]  ; Destination
    mov     edx, [ebp + 12] ; Char to be putted
    mov     ecx, [ebp + 16] ; Counter
.1:
    cmp     ecx, 0          ; 判断计数器
    jz      .2              ; 计数器为零时跳出

    mov     byte [edi], dl      ; ┓
    inc     edi                 ; ┛

    dec     ecx     ; 计数器减一
    jmp     .1      ; 循环
.2:

    pop     ecx
    pop     edi
    pop     esi
    mov     esp, ebp
    pop     ebp

    ret         ; 函数结束，返回
; ------------------------------------------------------------------------

; ------------------------------------------------------------------------
; char* strcpy(char* p_dst, char* p_src);
; ------------------------------------------------------------------------
strcpy:
    push    ebp
    mov     ebp, esp

    mov     esi, [ebp + 12] ; Source
    mov     edi, [ebp + 8]  ; Destination

.1:
    mov     al, [esi]               ; ┓
    inc     esi                     ; ┃
                                    ; ┣ 逐字节移动
    mov     byte [edi], al          ; ┃
    inc     edi                     ; ┛

    cmp     al, 0           ; 是否遇到 '\0'
    jnz     .1              ; 没遇到就继续循环，遇到就结束

    mov     eax, [ebp + 8]  ; 返回值

    pop     ebp
    ret                     ; 函数结束，返回
; strcpy 结束-------------------------------------------------------------


; ========================================================================
;                  void disable_irq(int irq);
; ========================================================================
; Disable an interrupt request line by setting an 8259 bit.
; Equivalent code:
;   if(irq < 8)
;       out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) | (1 << irq));
;   else
;       out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) | (1 << irq));
disable_irq:
        mov     ecx, [esp + 4]          ; irq
        pushf
        cli
        mov     ah, 1
        rol     ah, cl                  ; ah = (1 << (irq % 8))
        cmp     cl, 8
        jae     disable_8               ; disable irq >= 8 at the slave 8259
disable_0:
        in      al, INT_M_CTLMASK
        test    al, ah
        jnz     dis_already             ; already disabled?
        or      al, ah
        out     INT_M_CTLMASK, al       ; set bit at master 8259
        popf
        mov     eax, 1                  ; disabled by this function
        ret
disable_8:
        in      al, INT_S_CTLMASK
        test    al, ah
        jnz     dis_already             ; already disabled?
        or      al, ah
        out     INT_S_CTLMASK, al       ; set bit at slave 8259
        popf
        mov     eax, 1                  ; disabled by this function
        ret
dis_already:
        popf
        xor     eax, eax                ; already disabled
        ret

; ========================================================================
;                  void enable_irq(int irq);
; ========================================================================
; Enable an interrupt request line by clearing an 8259 bit.
; Equivalent code:
;       if(irq < 8)
;               out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) & ~(1 << irq));
;       else
;               out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) & ~(1 << irq));
;
enable_irq:
        mov     ecx, [esp + 4]          ; irq
        pushf
        cli
        mov     ah, ~1
        rol     ah, cl                  ; ah = ~(1 << (irq % 8))
        cmp     cl, 8
        jae     enable_8                ; enable irq >= 8 at the slave 8259
enable_0:
        in      al, INT_M_CTLMASK
        and     al, ah
        out     INT_M_CTLMASK, al       ; clear bit at master 8259
        popf
        ret
enable_8:
        in      al, INT_S_CTLMASK
        and     al, ah
        out     INT_S_CTLMASK, al       ; clear bit at slave 8259
        popf
        ret

; ========================================================================
;          void disable_int();
; ========================================================================
disable_int:
    cli
    ret

; ========================================================================
;          void enable_int();
; ========================================================================
enable_int:
    sti
    ret

    