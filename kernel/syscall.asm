; syscall.asm
; system call

%include "sconst.inc"

extern  sys_char


_NR_get_ticks       equ     0
_NR_write           equ     1
_NR_clscreen        equ     2
_NR_disable_tty     equ     3
_NR_enable_tty      equ     4
_NR_get_key         equ     5
INT_VECTOR_SYS_CALL equ     0x80

global  get_ticks
global  write
global  clscreen
global  disable_tty_output
global  enable_tty_output
global  get_key

bits 32

[section .text]

get_ticks;
        mov     eax, _NR_get_ticks
        int     INT_VECTOR_SYS_CALL
        ret


write:
        mov     eax, _NR_write
        mov     ebx, [esp + 4]
        mov     ecx, [esp + 8]
        int     INT_VECTOR_SYS_CALL
        ret

clscreen:
        mov     eax, _NR_clscreen
        int     INT_VECTOR_SYS_CALL
        ret

disable_tty_output:
        mov     eax, _NR_disable_tty
        int     INT_VECTOR_SYS_CALL
        ret

enable_tty_output:
        mov     eax, _NR_enable_tty
        int     INT_VECTOR_SYS_CALL
        ret

get_key:
        mov     eax, _NR_get_ticks
        int     INT_VECTOR_SYS_CALL
        mov     eax, [sys_char]
        ret        

