; compile method
;
; nasm -f elf foo.asm -o foo.o
; gcc -c bar.c -o bar.o
; ld -s hello.o bar.o -o foobar
; ./foobar

extern choose       ; int choose(int a, int b)

[section .data]

num1st      dd      3
num2nd      dd      4

[section .text]

global _start   ;  entry
global myprint

_start:
        ;push    dword   num2nd;   ; '.
        ;push    dword   num1st    ; |
        call    choose              ; choose(num1st, num2nd)
        add     esp, 8

        mov     ebx, 0
        mov     eax, 1
        int     0x80                ; system call

; void myprint(char* msg, int len)
myprint:
        mov     edx, [esp + 8]      ; len
        mov     ecx, [esp + 4]      ; msg
        mov     ebx, 1
        mov     eax, 4              ; sys_write
        int     0x80
        ret
