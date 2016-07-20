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
    print("#", Red);
    p_proc_ready++;
    if (p_proc_ready >= proc_table + NR_TASKS)
        p_proc_ready = proc_table;
}