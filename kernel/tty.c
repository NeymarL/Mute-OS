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
#include "console.h"
#include "tty.h"

#define TTY_FIRST (tty_table)
#define TTY_END (tty_table + NR_CONSOLES)

PRIVATE void tty_do_read(TTY* p_tty);
PRIVATE void tty_do_write(TTY* p_tty);

/*======================================================================*
                init_tty(TTY* p_tty)
 *======================================================================*/
PUBLIC void init_tty(TTY* p_tty)
{
    p_tty->inbuf_count = 0;
    p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;
    p_tty->disable_output = 0;
    init_screen(p_tty);
}


/*======================================================================*
                task_tty
 *======================================================================*/
PUBLIC void task_tty()
{
    TTY* p_tty;

    init_keyboard();

    for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++) {
        init_tty(p_tty);
    }

    select_console(0);
    while (1) {
        for (p_tty = TTY_FIRST; p_tty < TTY_END; p_tty++) {
            tty_do_read(p_tty);
            tty_do_write(p_tty);
        }
    }
}


/*======================================================================*
                in_process
 *======================================================================*/
PUBLIC void in_process(TTY* p_tty, u32 key)
{
        char output[2] = {'\0', '\0'};

        if (p_tty->disable_output) {
            sys_char = key;
            return;
        }

        if (!(key & FLAG_EXT)) {
            if (p_tty->inbuf_count < TTY_IN_BYTES) {
                *(p_tty->p_inbuf_head) = key;
                p_tty->p_inbuf_head++;
                if (p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES) {
                    p_tty->p_inbuf_head = p_tty->in_buf;
                }
                p_tty->inbuf_count++;
            }
        } else {
            int raw_code = key & MASK_RAW;
            switch(raw_code) {
                case UP:
                    if ((key & SHIFT_L) || (key & SHIFT_R)) {
                        scroll_screen(p_tty->p_console, SCR_UP);
                    }
                    break;
                case DOWN:
                    if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)) {
                        scroll_screen(p_tty->p_console, SCR_DN);
                    }
                    break;
                case F1:
                case F2:
                case F3:
                    if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R)) {
                        select_console(raw_code - F1);
                    }
                    break;
                case ENTER:
                    out_string(p_tty->p_console, "\n> ", Blue);
                    out_string(p_tty->p_console, "$ ", Light_Green);
                    break;
                case BACKSPACE:
                    out_char(p_tty->p_console, '\b', White);
                    break;
                default:
                    break;
            }
        }
}


PUBLIC void tty_write(TTY* p_tty, char* buf, int len)
{
    char* p = buf;
    int i = len;

    while (i > 0) {
        out_char(p_tty->p_console, *p++, White);
        i--;
    }
}

PUBLIC int sys_write(char* buf, int len, PROCESS* p_proc)
{
    //tty_write(&tty_table[nr_current_console], buf, len);
    tty_write(&tty_table[p_proc->nr_tty], buf, len);
    return 0;
}

PUBLIC void sys_disable_tty_output(PROCESS* p_proc)
{
    tty_table[p_proc->nr_tty].disable_output = 1;
}

PUBLIC void sys_enable_tty_output(PROCESS* p_proc)
{
    tty_table[p_proc->nr_tty].disable_output = 0;
}


PRIVATE void tty_do_read(TTY* p_tty)
{
    if (is_current_console(p_tty->p_console)) {
        keyboard_read(p_tty);
    }
}

PRIVATE void tty_do_write(TTY* p_tty)
{
    if (p_tty->inbuf_count && !p_tty->disable_output) {
        char ch = *(p_tty->p_inbuf_tail);
        p_tty->p_inbuf_tail++;
        if (p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES) {
            p_tty->p_inbuf_tail = p_tty->in_buf;
        }
        p_tty->inbuf_count--;
        
        out_char(p_tty->p_console, ch, DEFAULT_CHAR_COLOR);
    }
}


PUBLIC void sys_read_tty(PROCESS* p_proc)
{

}

