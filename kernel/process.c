/**
 * process.c
 * 实现一些系统调用
 */

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"
#include "process.h"


PUBLIC int sys_get_ticks()
{
    return ticks;
}


PUBLIC void schedule()
{
    PROCESS* p;
    int greatest_ticks = 0;

    while (!greatest_ticks) {
        for (p = proc_table; p < proc_table + NR_TASKS; p++) {
            if (p->ticks > greatest_ticks) {
                greatest_ticks = p->ticks;
                p_proc_ready = p;
            }
        }

        if (!greatest_ticks) {
            for (p = proc_table; p < proc_table + NR_TASKS; p++) {
                p->ticks = p->priority * TICK_BIAS;
            }
        }
    }
}

