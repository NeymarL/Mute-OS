/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _MUTEOS_CONST_H_
#define _MUTEOS_CONST_H_

/* EXTERN is defined as extern except in global.c */
#define EXTERN extern

/* 函数类型 */
#define PUBLIC          /* PUBLIC is the opposite of PRIVATE */
#define PRIVATE static  /* PRIVATE x limits the scope of x */

/* GDT 和 IDT 中描述符的个数 */
#define GDT_SIZE    128
#define IDT_SIZE    256

/* 权限 */
#define PRIVILEGE_KRNL  0
#define PRIVILEGE_TASK  1
#define PRIVILEGE_USER  3
/* RPL */
#define RPL_KRNL    SA_RPL0
#define RPL_TASK    SA_RPL1
#define RPL_USER    SA_RPL3
                            
/* 8259A interrupt controller ports. */
#define INT_M_CTL     0x20 /* I/O port for interrupt controller       <Master> */
#define INT_M_CTLMASK 0x21 /* setting bits in this port disables ints <Master> */
#define INT_S_CTL     0xA0 /* I/O port for second interrupt controller<Slave>  */
#define INT_S_CTLMASK 0xA1 /* setting bits in this port disables ints <Slave>  */



/* Define Color Scheme */
#define Black           0x00   
#define Blue            0x01   
#define Green           0x02   
#define Cyan            0x03   
#define Red             0x04   
#define Magenta         0x05   
#define Brown           0x06   
#define Light_Gray      0x07   
#define Dark_Gray       0x08   
#define Light_Blue      0x09   
#define Light_Green     0x0A   
#define Light_Cyan      0x0B   
#define Light_Red       0x0C   
#define Light_Magenta   0x0D   
#define Yellow          0x0E   
#define White           0x0F 


/* GDT */
/* 描述符索引 */
#define INDEX_DUMMY         0   // ┓
#define INDEX_FLAT_C        1   // ┣ LOADER 里面已经确定了的.
#define INDEX_FLAT_RW       2   // ┃
#define INDEX_VIDEO         3   // ┛
#define INDEX_TSS           4
#define INDEX_LDT_FIRST     5

/* 选择子 */
#define SELECTOR_DUMMY         0        // ┓
#define SELECTOR_FLAT_C     0x08        // ┣ LOADER 里面已经确定了的.
#define SELECTOR_FLAT_RW    0x10        // ┃
#define SELECTOR_VIDEO      (0x18+3)    // ┛<-- RPL=3
#define SELECTOR_TSS        0x20        /* TSS                       */
#define SELECTOR_LDT_FIRST  0x28

#define SELECTOR_KERNEL_CS  SELECTOR_FLAT_C
#define SELECTOR_KERNEL_DS  SELECTOR_FLAT_RW
#define SELECTOR_KERNEL_GS  SELECTOR_VIDEO

/* 每个任务有一个单独的 LDT, 每个 LDT 中的描述符个数: */
#define LDT_SIZE        2

/* 选择子类型值说明 */
/* 其中, SA_ : Selector Attribute */
#define SA_RPL_MASK 0xFFFC
#define SA_RPL0     0
#define SA_RPL1     1
#define SA_RPL2     2
#define SA_RPL3     3

#define SA_TI_MASK  0xFFFB
#define SA_TIG      0
#define SA_TIL      4

/* 描述符类型值说明 */
#define DA_32               0x4000  /* 32 位段              */
#define DA_64               0x2000  /* 64 位段              */
#define DA_LIMIT_4K         0x8000  /* 段界限粒度为 4K 字节  */
#define DA_DPL0             0x00    /* DPL = 0              */
#define DA_DPL1             0x20    /* DPL = 1              */
#define DA_DPL2             0x40    /* DPL = 2              */
#define DA_DPL3             0x60    /* DPL = 3              */
/* 存储段描述符类型值说明 */
#define DA_DR               0x90    /* 存在的只读数据段类型值         */
#define DA_DRW              0x92    /* 存在的可读写数据段属性值        */
#define DA_DRWA             0x93    /* 存在的已访问可读写数据段类型值 */
#define DA_C                0x98    /* 存在的只执行代码段属性值     */
#define DA_CR               0x9A    /* 存在的可执行可读代码段属性值       */
#define DA_CCO              0x9C    /* 存在的只执行一致代码段属性值       */
#define DA_CCOR             0x9E    /* 存在的可执行可读一致代码段属性值 */
/* 系统段描述符类型值说明 */
#define DA_LDT              0x82    /* 局部描述符表段类型值           */
#define DA_TaskGate         0x85    /* 任务门类型值               */
#define DA_386TSS           0x89    /* 可用 386 任务状态段类型值      */
#define DA_386CGate         0x8C    /* 386 调用门类型值           */
#define DA_386IGate         0x8E    /* 386 中断门类型值           */
#define DA_386TGate         0x8F    /* 386 陷阱门类型值           */

/* 中断向量 */
#define INT_VECTOR_DIVIDE           0x0
#define INT_VECTOR_DEBUG            0x1
#define INT_VECTOR_NMI              0x2
#define INT_VECTOR_BREAKPOINT       0x3
#define INT_VECTOR_OVERFLOW         0x4
#define INT_VECTOR_BOUNDS           0x5
#define INT_VECTOR_INVAL_OP         0x6
#define INT_VECTOR_COPROC_NOT       0x7
#define INT_VECTOR_DOUBLE_FAULT     0x8
#define INT_VECTOR_COPROC_SEG       0x9
#define INT_VECTOR_INVAL_TSS        0xA
#define INT_VECTOR_SEG_NOT          0xB
#define INT_VECTOR_STACK_FAULT      0xC
#define INT_VECTOR_PROTECTION       0xD
#define INT_VECTOR_PAGE_FAULT       0xE
#define INT_VECTOR_COPROC_ERR       0x10
#define INT_VECTOR_SYS_CALL         0x80

/* 中断向量 */
#define INT_VECTOR_IRQ0             0x20
#define INT_VECTOR_IRQ8             0x28

/* 宏 */
/* 线性地址 → 物理地址 */
#define vir2phys(seg_base, vir) (u32)(((u32)(seg_base)) + (u32)(vir))

/* Hardware interrupts */
#define NR_IRQ          16  /* Number of IRQs */
#define CLOCK_IRQ       0
#define KEYBOARD_IRQ    1
#define CASCADE_IRQ     2   /* cascade enable for 2nd AT controller */
#define ETHER_IRQ       3   /* default ethernet interrupt vector */
#define SECONDARY_IRQ   3   /* RS232 interrupt vector for port 2 */
#define RS232_IRQ       4   /* RS232 interrupt vector for port 1 */
#define XT_WINI_IRQ     5   /* xt winchester */
#define FLOPPY_IRQ      6   /* floppy disk */
#define PRINTER_IRQ     7
#define AT_WINI_IRQ     14  /* at winchester */

/* system call */
#define NR_SYS_CALL     6

/* 8253/8254 PIT (Programmable Interval Timer) */
#define TIMER0         0x40     /* I/O port for timer channel 0 */
#define TIMER_MODE     0x43     /* I/O port for timer mode control */
#define RATE_GENERATOR 0x34     /* 00-11-010-0 :
                                 * Counter0 - LSB then MSB - rate generator - binary
                                 */
#define TIMER_FREQ     1193182L /* clock frequency for timer in PC and AT */
#define HZ             100      /* clock freq (software settable on IBM-PC) */

#define TICK_BIAS      82       /* 为了凑一秒钟而必须乘的系数 */

/* AT keyboard */
/* 8042 ports */
#define KB_DATA     0x60    /* I/O port for keyboard data
                                Read : Read Output Buffer
                                Write: Write Input Buffer(8042 Data&8048 Command) */
#define KB_CMD      0x64    /* I/O port for keyboard command
                                Read : Read Status Register
                                Write: Write Input Buffer(8042 Command) */
#define LED_CODE    0xED
#define KB_ACK      0xFA


/* VGA */
#define CRTC_ADDR_REG   0x3D4   /* CRT Controller Registers - Addr Register */
#define CRTC_DATA_REG   0x3D5   /* CRT Controller Registers - Data Register */
#define START_ADDR_H    0xC     /* reg index of video mem start addr (MSB) */
#define START_ADDR_L    0xD     /* reg index of video mem start addr (LSB) */
#define CURSOR_H        0xE     /* reg index of cursor position (MSB) */
#define CURSOR_L        0xF     /* reg index of cursor position (LSB) */
#define V_MEM_BASE      0xB8000 /* base of color video memory */
#define V_MEM_SIZE      0x8000  /* 32K: B8000H -> BFFFFH */


/* TTY */
#define NR_CONSOLES     3


#endif /* _MUTEOS_CONST_H_ */
