; func
; TAB=4

[BITS 32]               ; 制作32位模式用的机器语言

        GLOBAL      io_hlt, io_cli, io_sti, io_stihlt  
        GLOBAL      write_mem8
        GLOBAL      io_in8, io_in16, io_in32
        GLOBAL      io_out8, io_out16, io_out32
        GLOBAL      io_load_eflags, io_store_eflags

; 以下是实际函数

io_hlt:     ; void io_hlt();
    HLT
    RET

write_mem8: ; void write_mem8(int addr, int data);
    MOV     ECX, EDI
    MOV     EAX, 0
    MOV     AX, SI
    ;MOV     ECX, [EBP + 8]
    ;MOV     EAX, [EBP + 12]
    MOV     [ECX], AL
    RET

io_cli:     ; void io_cli();
    CLI 
    RET

io_sti:     ; void io_sti();
    STI 
    RET

io_stihlt:  ; void io_stihlt();
    STI 
    HLT 
    RET

io_in8:     ; int io_in8(int port);
    MOV     EDX, EDI ; port
    MOV     EAX, 0
    IN      AL, DX
    RET

io_in16:    ; int io_in16(int port);
    MOV     EDX, EDI ; port
    MOV     EAX, 0
    IN      AX, DX
    RET             ; 执行RET语句时，EAX的值就被看作是函数的返回值

io_in32:    ; int io_in32(int port);
    MOV     EDX, EDI ; port
    IN      EAX, DX
    RET

io_out8:   ; void io_out8(int port, int data);
    MOV     EDX, EDI ; port
    MOV     EAX, 0
    MOV     AX, SI          ; data
    OUT     DX, AX
    RET

io_out16:   ; void io_out16(int port, int data);
    MOV     EDX, EDI ; port
    MOV     EAX, 0
    MOV     AX, SI          ; data
    OUT     DX, AX
    RET

io_out32:   ; void io_out32(int port, int data);
    MOV     EDX, EDI ; port
    MOV     EAX, 0
    MOV     EAX, ESI        ; data
    OUT     DX, EAX
    RET

io_load_eflags: ; int io_load_eflags();
    PUSHFD      ; push flags double word
    POP     EAX
    RET

io_store_eflags:    ; void io_store_eflags();
    MOV     EAX, EDI
    PUSH    EAX
    POPFD           ; pop flags double word
    RET

