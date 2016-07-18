; lib.asm
;

[BITS 64]

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

; ========================================================================
;                  void out_byte(u16 port, u8 value);
; ========================================================================
out_byte:
        mov     edx, edi        ; port
        mov     eax, esi        ; value
        ;shl     eax, 8
        out     dx, al
        nop                     ; 一点延迟
        nop
        ret

; ========================================================================
;                  u8 in_byte(u16 port);
; ========================================================================
in_byte:
        mov     edx, edi            ; port
        xor     eax, eax
        in      al, dx
        nop                         ; 一点延迟
        nop
        ret
; ========================================================================

; ========================================================================
;                 void init_8259A_asm();
; ========================================================================
init_8259A_asm:
        mov     al, 0x11
        out     0x20, al
        nop
        nop
        mov     al, 0x11
        out     0xA0, al
        nop
        nop
        mov     al, 0x20
        out     0x21, al
        nop
        nop
        mov     al, 0x18
        out     0xA1, al
        nop
        nop
        mov     al, 0x4
        out     0x21, al
        nop
        nop
        mov     al, 0x2
        out     0xA1, al
        nop
        nop
        mov     al, 0x1
        out     0x21, al
        nop
        nop
        mov     al, 0x1
        out     0xA1, al
        nop
        nop
        mov     al, 0xFD
        out     0x21, al
        nop
        nop
        mov     al, 0xFF
        out     0xA1, al
        nop
        nop
        ret


[BITS 32]
; ------------------------------------------------------------------------
; 显示 AL 中的数字
; ------------------------------------------------------------------------
DispAL:
    push    ecx
    push    edx
    push    edi

    mov edi, [disp_pos]

    mov ah, 0Fh         ; 0000b: 黑底    1111b: 白字
    mov dl, al
    shr al, 4
    mov ecx, 2
.begin:
    and al, 01111b
    cmp al, 9
    ja  .1
    add al, '0'
    jmp .2
.1:
    sub al, 0Ah
    add al, 'A'
.2:
    mov [gs:edi], ax
    add edi, 2

    mov al, dl
    loop    .begin
    ;add    edi, 2

    mov [disp_pos], edi

    pop edi
    pop edx
    pop ecx

    ret
; DispAL 结束-------------------------------------------------------------


; ------------------------------------------------------------------------
; 显示一个整形数
; ------------------------------------------------------------------------
DispInt:
    mov eax, [esp + 4]
    shr eax, 24
    call    DispAL

    mov eax, [esp + 4]
    shr eax, 16
    call    DispAL

    mov eax, [esp + 4]
    shr eax, 8
    call    DispAL

    mov eax, [esp + 4]
    call    DispAL

    mov ah, 07h         ; 0000b: 黑底    0111b: 灰字
    mov al, 'h'
    push    edi
    mov edi, [disp_pos]
    mov [gs:edi], ax
    add edi, 4
    mov [disp_pos], edi
    pop edi

    ret
; DispInt 结束------------------------------------------------------------


;------------------------------------------------------
; set_interrupt_handler(u16 vector, u8 desc_type,
;             int_handler handler, unsigned char privilege)
; input:
;   rdi: vector,  rsi: desc_type, rdx : handler, rcx : privilege
;------------------------------------------------------
;set_interrupt_handler:
;        sidt [idt_ptr]        
;        mov rax, [idt_ptr + 2]                                  ; IDT base
;        shl rdi, 4                                              ; vector * 16
;        add rax, rdi
;        mov [rax], rdx                                    ; offset [15:0]
;        mov [rax + 4], rdx                                ; offset [63:16]
;        mov DWORD [rax + 2], SELECTOR_KERNEL_CS           ; set selector
;        shl rcx, 5
;        mov BYTE [rax + 5], rsi | rcx                     ; Type=interrupt gate, P=1, DPL=0
;        ret

