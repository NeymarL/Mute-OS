/*
 * clock.c
 */

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"
#include "process.h"


PUBLIC void clock_handler(int irq)
{
    ticks++;

    if (k_reenter != 0) {
        return;
    }

    p_proc_ready++;
    if (p_proc_ready >= proc_table + NR_TASKS)
        p_proc_ready = proc_table;
}

PUBLIC void mili_delay(int milli_sec)
{
    int t = get_ticks();
    while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

PUBLIC void delay(int sec)
{
    mili_delay(82000 * sec);
}
