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
    p_proc_ready->ticks--;

    if (k_reenter != 0) {
        return;
    }

    if (p_proc_ready->ticks > 0) {
        return;
    }

    schedule();

}

PUBLIC void mili_delay(int milli_sec)
{
    int t = get_ticks();
    while(((get_ticks() - t) * 1000 / HZ) < milli_sec * TICK_BIAS) {}
}

PUBLIC void delay(int sec)
{
    mili_delay(1000 * sec);
}
