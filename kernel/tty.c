/**
 * tty.c
 * 终端任务
 */

#include "type.h"
#include "const.h"
#include "global.h"
#include "func.h"
#include "process.h"
#include "keyboard.h"

PUBLIC void init_tty()
{
    mili_delay(300);
    clear_screen();
    print("MuteOS> ", White);
}


PUBLIC void task_tty()
{
    init_tty();
    while (1) {
        keyboard_read();
    }
}