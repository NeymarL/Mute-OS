/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"
#include "process.h"
#include "console.h"
#include "tty.h"


PUBLIC  PROCESS         proc_table[NR_TASKS + NR_PROCS];

PUBLIC  char            task_stack[STACK_SIZE_TOTAL];

PUBLIC  TASK task_table[NR_TASKS] = {
    {task_tty, STACK_SIZE_TTY, "tty1", 15 }
};

PUBLIC  TASK user_proc_table[NR_PROCS] = {
    {TestB, STACK_SIZE_TESTB, "TestB", 10 }
};


PUBLIC  irq_handler     irq_table[NR_IRQ];

PUBLIC  system_call     sys_call_table[NR_SYS_CALL] = {
    sys_get_ticks,
    sys_write
};

PUBLIC  TTY             tty_table[NR_CONSOLES];
PUBLIC  CONSOLE         console_table[NR_CONSOLES];
