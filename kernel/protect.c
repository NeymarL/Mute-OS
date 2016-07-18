/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              protect.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "global.h"
#include "func.h"

/* 本文件内函数声明 */
PRIVATE void init_idt_desc(u16 vector, u8 desc_type,
               int_handler handler, unsigned char privilege);


/* 中断处理函数 */
void    divide_error();
void    single_step_exception();
void    nmi();
void    breakpoint_exception();
void    overflow();
void    bounds_check();
void    inval_opcode();
void    copr_not_available();
void    double_fault();
void    copr_seg_overrun();
void    inval_tss();
void    segment_not_present();
void    stack_exception();
void    general_protection();
void    page_fault();
void    copr_error();
void    hwint00();
void    hwint01();
void    hwint02();
void    hwint03();
void    hwint04();
void    hwint05();
void    hwint06();
void    hwint07();
void    hwint08();
void    hwint09();
void    hwint10();
void    hwint11();
void    hwint12();
void    hwint13();
void    hwint14();
void    hwint15();


/*======================================================================*
                            init_8259A
 *======================================================================*/
PUBLIC void init_8259A()
{
    /* Master 8259, ICW1. */
    out_byte(INT_M_CTL, 0x11);

    /* Slave  8259, ICW1. */
    out_byte(INT_S_CTL, 0x11);

    /* Master 8259, ICW2. 设置 '主8259' 的中断入口地址为 0x20. */
    out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0);

    /* Slave  8259, ICW2. 设置 '从8259' 的中断入口地址为 0x28 */
    out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8);

    /* Master 8259, ICW3. IR2 对应 '从8259'. */
    out_byte(INT_M_CTLMASK, 0x4);

    /* Slave  8259, ICW3. 对应 '主8259' 的 IR2. */
    out_byte(INT_S_CTLMASK, 0x2);

    /* Master 8259, ICW4. */
    out_byte(INT_M_CTLMASK, 0x1);

    /* Slave  8259, ICW4. */
    out_byte(INT_S_CTLMASK, 0x1);

    /* Master 8259, OCW1.  */
    out_byte(INT_M_CTLMASK, 0xFD);

    /* Slave  8259, OCW1.  */
    out_byte(INT_S_CTLMASK, 0xFF);
}


/*======================================================================*
                            init_prot
 *======================================================================*/
PUBLIC void init_prot()
{
    init_8259A();

    // 全部初始化成中断门(没有陷阱门)
    init_idt_desc(INT_VECTOR_DIVIDE,    DA_386IGate,
              divide_error,     PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_DEBUG,     DA_386IGate,
              single_step_exception,    PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_NMI,       DA_386IGate,
              nmi,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_BREAKPOINT,    DA_386IGate,
              breakpoint_exception, PRIVILEGE_USER);

    init_idt_desc(INT_VECTOR_OVERFLOW,  DA_386IGate,
              overflow,         PRIVILEGE_USER);

    init_idt_desc(INT_VECTOR_BOUNDS,    DA_386IGate,
              bounds_check,     PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_INVAL_OP,  DA_386IGate,
              inval_opcode,     PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_COPROC_NOT,    DA_386IGate,
              copr_not_available,   PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_DOUBLE_FAULT,  DA_386IGate,
              double_fault,     PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_COPROC_SEG,    DA_386IGate,
              copr_seg_overrun,     PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_INVAL_TSS, DA_386IGate,
              inval_tss,        PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_SEG_NOT,   DA_386IGate,
              segment_not_present,  PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_STACK_FAULT,   DA_386IGate,
              stack_exception,      PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_PROTECTION,    DA_386IGate,
              general_protection,   PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_PAGE_FAULT,    DA_386IGate,
              page_fault,       PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_COPROC_ERR,    DA_386IGate,
              copr_error,       PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 0,      DA_386IGate,
              hwint00,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 1,      DA_386IGate,
              hwint01,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 2,      DA_386IGate,
              hwint02,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 3,      DA_386IGate,
              hwint03,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 4,      DA_386IGate,
              hwint04,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 5,      DA_386IGate,
              hwint05,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 6,      DA_386IGate,
              hwint06,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ0 + 7,      DA_386IGate,
              hwint07,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 0,      DA_386IGate,
              hwint08,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 1,      DA_386IGate,
              hwint09,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 2,      DA_386IGate,
              hwint10,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 3,      DA_386IGate,
              hwint11,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 4,      DA_386IGate,
              hwint12,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 5,      DA_386IGate,
              hwint13,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 6,      DA_386IGate,
              hwint14,          PRIVILEGE_KRNL);

    init_idt_desc(INT_VECTOR_IRQ8 + 7,      DA_386IGate,
              hwint15,          PRIVILEGE_KRNL);
}

/*======================================================================*
                             init_idt_desc
 *----------------------------------------------------------------------*
 初始化 386 中断门
 *======================================================================*/
PRIVATE void init_idt_desc(u16 vector, u8 desc_type,
              int_handler handler, unsigned char privilege)
{
    GATE *  p_gate  = &idt[vector];
    //u32 base    = (u32)handler;
    u64 base    = (u64)handler;
    p_gate->offset_low  = base & 0xFFFF;
    p_gate->selector    = SELECTOR_KERNEL_CS;
    //p_gate->dcount      = 0;
    p_gate->ist         = 0;
    p_gate->attr        = desc_type | (privilege << 5);
    p_gate->offset_mid  = (base >> 16) & 0xFFFF;
    p_gate->offset_high = (base >> 32) & 0x0FFFFFFFF;
    //p_gate->unused      = 0;
}

/*======================================================================*
                            exception_handler
 *----------------------------------------------------------------------*
 异常处理
 *======================================================================*/
PUBLIC void exception_handler(int vec_no, unsigned int err_code)
{
    int i;
    char text_color = Red;

    char * err_msg[] = {
                "#DE Divide Error",
                "#DB RESERVED",
                "--  NMI Interrupt",
                "#BP Breakpoint",
                "#OF Overflow",
                "#BR BOUND Range Exceeded",
                "#UD Invalid Opcode (Undefined Opcode)",
                "#NM Device Not Available (No Math Coprocessor)",
                "#DF Double Fault",
                "    Coprocessor Segment Overrun (reserved)",
                "#TS Invalid TSS",
                "#NP Segment Not Present",
                "#SS Stack-Segment Fault",
                "#GP General Protection",
                "#PF Page Fault",
                "--  (Intel reserved. Do not use.)",
                "#MF x87 FPU Floating-Point Error (Math Fault)",
                "#AC Alignment Check",
                "#MC Machine Check",
                "#XF SIMD Floating-Point Exception"
    };

    /* 通过打印空格的方式清空屏幕的前五行，并把 disp_pos 清零 */
    /*disp_pos = 0;
    for (i = 0; i < 80 * 5; i++) {
        print(" ", Black);
    }*/

    print("\nException! --> ", text_color);
    print(err_msg[vec_no], text_color);   // Problem!!!!
    print("\n", text_color);

    /*print("EFLAGS:", text_color);
    print_bit(eflags, text_color);

    print("CS:", text_color);
    print_bit(cs, text_color);

    print("EIP:", text_color);
    print_bit(eip, text_color);*/

    if (err_code != 0xFFFFFFFF){
        print("Error code:", text_color);
        print_bit(err_code, text_color);
    }
}

/*======================================================================*
                           spurious_irq
 *======================================================================*/
PUBLIC void spurious_irq(int irq)
{
        print("spurious_irq: ", White);
        print_bit(irq, White);
        print("\n", White);
}
