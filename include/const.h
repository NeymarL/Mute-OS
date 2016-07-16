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

/* 8259A interrupt controller ports. */
#define INT_M_CTL     0x20 /* I/O port for interrupt controller       <Master> */
#define INT_M_CTLMASK 0x21 /* setting bits in this port disables ints <Master> */
#define INT_S_CTL     0xA0 /* I/O port for second interrupt controller<Slave>  */
#define INT_S_CTLMASK 0xA1 /* setting bits in this port disables ints <Slave>  */


/* Define Color Scheme */
#define Black           0x01   
#define Blue            0x02   
#define Green           0x03   
#define Cyan            0x04   
#define Red             0x05   
#define Magenta         0x06   
#define Brown           0x07   
#define Light_Gray      0x08   
#define Dark_Gray       0x09   
#define Light_Blue      0x0A   
#define Light_Green     0x0B   
#define Light_Cyan      0x0C   
#define Light_Red       0x0D   
#define Light_Magenta   0x0E   
#define Yellow          0x0F   
#define White           0x10 


/* GDT */
/* 描述符索引 */
#define INDEX_DUMMY         0   // ┓
#define INDEX_FLAT_C        1   // ┣ LOADER 里面已经确定了的.
#define INDEX_FLAT_RW       2   // ┃
#define INDEX_VIDEO         3   // ┛
/* 选择子 */
#define SELECTOR_DUMMY         0        // ┓
#define SELECTOR_FLAT_C     0x08        // ┣ LOADER 里面已经确定了的.
#define SELECTOR_FLAT_RW    0x10        // ┃
#define SELECTOR_VIDEO      (0x18+3)    // ┛<-- RPL=3

#define SELECTOR_KERNEL_CS  SELECTOR_FLAT_C
#define SELECTOR_KERNEL_DS  SELECTOR_FLAT_RW


/* 描述符类型值说明 */
#define DA_32               0x4000  /* 32 位段              */
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

/* 中断向量 */
#define INT_VECTOR_IRQ0             0x20
#define INT_VECTOR_IRQ8             0x28

#endif /* _MUTEOS_CONST_H_ */