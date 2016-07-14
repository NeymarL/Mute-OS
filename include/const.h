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


#endif /* _MUTEOS_CONST_H_ */
