
; ------------------------------------------------------------------------
; 显示 AL 中的数字
; ------------------------------------------------------------------------
DispAL:
    push    rcx
    push    rdx
    push    rdi

    mov edi, [dwDispPos]

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

    mov [dwDispPos], edi

    pop rdi
    pop rdx
    pop rcx

    ret
; DispAL 结束-------------------------------------------------------------


; ------------------------------------------------------------------------
; 显示一个整形数
; ------------------------------------------------------------------------
DispInt:
    mov     eax, ecx
    shr     eax, 24
    call    DispAL

    mov     eax, ecx
    shr     eax, 16
    call    DispAL

    mov     eax, ecx
    shr     eax, 8
    call    DispAL

    mov     eax, ecx
    call    DispAL

    mov     ah, 07h         ; 0000b: 黑底    0111b: 灰字
    mov     al, 'h'
    push    rdi
    mov     edi, [dwDispPos]
    mov     [gs:edi], ax
    add     edi, 4
    mov     [dwDispPos], edi
    pop     rdi

    ret
; DispInt 结束------------------------------------------------------------

; ------------------------------------------------------------------------
; 显示一个字符串
; ------------------------------------------------------------------------
DispStr:
    push    rbp
    mov     ebp, esp
    push    rbx
    push    rsi
    push    rdi

    mov     rsi, rcx  ; pszInfo
    mov     edi, [dwDispPos]
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
    mov     [dwDispPos], edi

    pop     rdi
    pop     rsi
    pop     rbx
    pop     rbp
    ret
; DispStr 结束------------------------------------------------------------

; ------------------------------------------------------------------------
; 换行
; ------------------------------------------------------------------------
DispReturn:
    ;push    szReturn
    mov     ecx, szReturn
    call    DispStr         ;printf("\n");
    add esp, 4

    ret
; DispReturn 结束---------------------------------------------------------


; ------------------------------------------------------------------------
; 内存拷贝，仿 memcpy
; ------------------------------------------------------------------------
; void* MemCpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
MemCpy:
    push    rbp
    mov ebp, esp

    push    rsi
    push    rdi
    push    rcx

    mov edi, [ebp + 8]  ; Destination
    mov esi, [ebp + 12] ; Source
    mov ecx, [ebp + 16] ; Counter
.1:
    cmp ecx, 0      ; 判断计数器
    jz  .2      ; 计数器为零时跳出

    mov al, [ds:esi]        ; ┓
    inc esi                 ; ┃
                            ; ┣ 逐字节移动
    mov byte [es:edi], al   ; ┃
    inc edi                 ; ┛

    dec ecx     ; 计数器减一
    jmp .1      ; 循环
.2:
    mov eax, [ebp + 8]  ; 返回值

    pop rcx
    pop rdi
    pop rsi
    mov esp, ebp
    pop rbp

    ret         ; 函数结束，返回
; MemCpy 结束-------------------------------------------------------------

