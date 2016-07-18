/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _MUTEOS_TYPE_H_
#define _MUTEOS_TYPE_H_

typedef unsigned long long  u64;
typedef unsigned int        u32;
typedef unsigned short      u16;
typedef unsigned char       u8;

typedef void  (*int_handler)  ();

/* 存储段描述符/系统段描述符 */
typedef struct s_descriptor     /* 共 8 个字节 */
{
    u16 limit_low;              /* Limit */
    u16 base_low;               /* Base */
    u8  base_mid;               /* Base */
    u8  attr1;                  /* P(1) DPL(2) DT(1) TYPE(4) */
    u8  limit_high_attr2;       /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
    u8  base_high;              /* Base */
}DESCRIPTOR;

/* 门描述符 */
typedef struct s_gate   /* 16 Bytes */
{
    u16 offset_low;     /* Offset Low */
    u16 selector;       /* Selector */
    u8  ist;            /* Interrupt Stack Table */
    u8  attr;           /* P(1) DPL(2) DT(1) TYPE(4) */
    u16 offset_mid;     /* Offset Mid */
    u32 offset_high;    /* Offset High */
    //u32 unused;         /* Unused */
} GATE;


#endif /* _MUTEOS_TYPE_H_ */
