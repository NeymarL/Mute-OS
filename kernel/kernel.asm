; kernel.asm
;

SELECTOR_KERNEL_CS  equ 8

; 导入函数
extern  cstart

; 导入全局变量
extern  gdt_ptr


[SECTION .bss]
StackSpace      resb    2 * 1024
StackTop:       ; 栈顶


[section .text] 

global _start   ; 导出 _start

_start: ; 
    ;
    ; GDT 以及相应的描述符是这样的：
    ;
    ;                     Descriptors               Selectors
    ;              ┏━━━━━━━━━━━━━━━━━━┓
    ;              ┃         Dummy Descriptor           ┃
    ;              ┣━━━━━━━━━━━━━━━━━━┫
    ;              ┃         DESC_FLAT_C    (0～4G)     ┃   8h = cs
    ;              ┣━━━━━━━━━━━━━━━━━━┫
    ;              ┃         DESC_FLAT_RW   (0～4G)     ┃  10h = ds, es, fs, ss
    ;              ┣━━━━━━━━━━━━━━━━━━┫
    ;              ┃         DESC_VIDEO                 ┃  1Bh = gs
    ;              ┗━━━━━━━━━━━━━━━━━━┛
    ;
    ; 注意! 在使用 C 代码的时候一定要保证 ds, es, ss 这几个段寄存器的值是一样的
    ; 因为编译器有可能编译出使用它们的代码, 而编译器默认它们是一样的. 比如串拷贝操作会用到 ds 和 es.
    ;
    ;
    ; 把 esp 从 LOADER 挪到 KERNEL
    mov     esp, StackTop   ; 堆栈在 bss 段中

    sgdt    [gdt_ptr]   ; cstart() 中将会用到 gdt_ptr
    call    cstart      ; 在此函数中改变了gdt_ptr，让它指向新的GDT
    lgdt    [gdt_ptr]   ; 使用新的GDT

    ;lidt   [idt_ptr]

    ;jmp     SELECTOR_KERNEL_CS:csinit
    push    word SELECTOR_KERNEL_CS
    push    qword csinit
    retf

csinit:     ; “这个跳转指令强制使用刚刚初始化的结构”——<<OS:D&I 2nd>> P90.

    push    0
    popfq   ; Pop top of stack into EFLAGS

    hlt
