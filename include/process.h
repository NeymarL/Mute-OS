/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                        process.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _MUTEOS_PROCESS_H_
#define _MUTEOS_PROCESS_H_

#include "const.h"

typedef struct s_stackframe {   /*               ↑ Low        */
    u32 gs;         /* ┓                        │           */
    u32 fs;         /* ┃                        │           */
    u32 es;         /* ┃                        │           */
    u32 ds;         /* ┃                        │           */
    u32 edi;        /* ┃                        │           */
    u32 esi;        /* ┣ pushed by save()       │           */
    u32 ebp;        /* ┃                        │           */
    u32 kernel_esp; /* <- 'popad' will ignore it │           */
    u32 ebx;        /* ┃                        ↑栈从高地址往低地址增长*/      
    u32 edx;        /* ┃                        │           */
    u32 ecx;        /* ┃                        │           */
    u32 eax;        /* ┛                        │           */
    u32 retaddr;    /* return address save()     │           */
    u32 eip;        /*  ┓                       │           */
    u32 cs;         /*  ┃                       │           */
    u32 eflags;     /*  ┣ these are pushed by CPU during interrupt */
    u32 esp;        /*  ┃                       │           */
    u32 ss;         /*  ┛                       ┷High           */
} STACK_FRAME;


typedef struct s_proc {
    STACK_FRAME regs;          /* process registers saved in stack frame */
    u16 ldt_sel;               /* gdt selector giving ldt base and limit */
    DESCRIPTOR ldts[LDT_SIZE]; /* local descriptors for code and data */
    int ticks;                 /* remained ticks */
    int priority;
    u32 pid;                   /* process id passed in from MM */
    char p_name[16];           /* name of the process */
} PROCESS;


typedef struct s_task {
    task_f  initial_eip;
    int     stacksize;
    char    name[32];
    int     priority;
} TASK;


/* Number of tasks */
#define NR_TASKS    1

/* stacks of tasks */
#define STACK_SIZE_TTY      0x8000
#define STACK_SIZE_TESTB    0x8000

#define STACK_SIZE_TOTAL    (STACK_SIZE_TTY + \
                STACK_SIZE_TESTB)

#endif /* _MUTEOS_PROCESS_H_ */
