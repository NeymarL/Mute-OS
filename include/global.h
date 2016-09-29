/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.h

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* EXTERN is defined as extern except in global.c */
#ifdef  GLOBAL_VARIABLES_HERE
#undef  EXTERN
#define EXTERN
#endif

#include "process.h"
#include "console.h"
#include "tty.h"


EXTERN  int         disp_pos;
EXTERN  u8          gdt_ptr[6]; /* 0~15:Limit  16~47:Base */
EXTERN  DESCRIPTOR  gdt[GDT_SIZE];
EXTERN  u8          idt_ptr[6]; /* 0~15:Limit  16~79:Base */
EXTERN  GATE        idt[IDT_SIZE];

EXTERN  TSS         tss;
EXTERN  PROCESS*    p_proc_ready;

EXTERN  int         k_reenter;
EXTERN  int         ticks;

EXTERN  int         nr_current_console;

EXTERN  int         sys_char;

extern  PROCESS     proc_table[];
extern  char        task_stack[];
extern  TASK        task_table[];
extern  irq_handler irq_table[];
extern  TTY         tty_table[];
extern  CONSOLE     console_table[];
extern  TASK        user_proc_table[];

