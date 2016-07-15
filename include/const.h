/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _MUTEOS_CONST_H_
#define _MUTEOS_CONST_H_


/* 函数类型 */
#define PUBLIC          /* PUBLIC is the opposite of PRIVATE */
#define PRIVATE static  /* PRIVATE x limits the scope of x */

/* GDT 和 IDT 中描述符的个数 */
#define GDT_SIZE    128

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


#endif /* _MUTEOS_CONST_H_ */
