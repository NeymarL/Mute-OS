/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"
#include "process.h"


/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
    print("[ OK ]", Green);
    print(" kernel_main begins\n", White);

    TASK*      p_task       = task_table;
    PROCESS*   p_proc       = proc_table;
    char*      p_task_stack = task_stack + STACK_SIZE_TOTAL;
    u16        selector_ldt = SELECTOR_LDT_FIRST;
    u8         privilege;
    u8         rpl;
    int        eflags;
    for (int i = 0; i < NR_TASKS + NR_PROCS; i++) {
        if (i < NR_TASKS) {
            // tasks
            p_task = task_table + i;
            privilege = PRIVILEGE_TASK;
            rpl = RPL_TASK;
            eflags = 0x1202;            /* IF=1, IOPL=1, bit2 is always 1 */   
        } 
        else {
            // user processes
            p_task = user_proc_table + (i - NR_TASKS);
            privilege = PRIVILEGE_USER;
            rpl = RPL_USER;
            eflags = 0x202;             /* IF=1, bit2 is always 1 */ 
        }

        strcpy(p_proc->p_name, p_task->name);   // name of the process
        p_proc->pid = i;                        // pid

        p_proc->ldt_sel = selector_ldt;

        memocpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
               sizeof(DESCRIPTOR));
        p_proc->ldts[0].attr1 = DA_C | privilege << 5;
        memocpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
               sizeof(DESCRIPTOR));
        p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
        p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK)
            | rpl;

        p_proc->regs.eip = (u32)p_task->initial_eip;
        p_proc->regs.esp = (u32)p_task_stack;
        p_proc->regs.eflags = eflags;

        p_task_stack -= p_task->stacksize;
 
        /* init priority */
        proc_table[i].priority = p_task->priority;
        proc_table[i].ticks = proc_table[i].priority * TICK_BIAS;
        proc_table[i].nr_tty = 0;

        p_proc++;
        p_task++;
        selector_ldt += 1 << 3;
    }

    k_reenter = 0;
    ticks = 0;

    init_clock();    
    init_keyboard();

    p_proc_ready    = proc_table; 
    restart();

    while(1){}
}


/*======================================================================*
                               init_clock
 *======================================================================*/
PUBLIC void init_clock()
{
    /* initialize 8253 PIT */
    out_byte(TIMER_MODE, RATE_GENERATOR);
    out_byte(TIMER0, (u8)(TIMER_FREQ / HZ));
    out_byte(TIMER0, (u8)(TIMER_FREQ / HZ) >> 8);

    put_irq_handler(CLOCK_IRQ, clock_handler);  /* 设定时钟中断处理程序 */
    enable_irq(CLOCK_IRQ);                      /* 让8259A可以接收时钟中断 */    
}


/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
    while(1){
        //out_char(&console_table[nr_current_console], 'c', White);
        printf("<Ticks %d>", get_ticks());
        mili_delay(500);
    }
}

