/**
 * console.c
 */

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"
#include "process.h"
#include "console.h"
#include "tty.h"


PRIVATE void set_cursor(unsigned int position);

PUBLIC int is_current_console(CONSOLE* p_con)
{
    return (p_con == &console_table[nr_current_console]);
}

/*======================================================================*
               init_screen
 *======================================================================*/
PUBLIC void init_screen(TTY* p_tty)
{
    int nr_tty = p_tty - tty_table;
    p_tty->p_console = console_table + nr_tty;

    int v_mem_size = V_MEM_SIZE >> 1;   /* 显存总大小 (in WORD) */

    int con_v_mem_size                   = v_mem_size / NR_CONSOLES;
    p_tty->p_console->original_addr      = nr_tty * con_v_mem_size;
    p_tty->p_console->v_mem_limit        = con_v_mem_size;
    p_tty->p_console->current_start_addr = p_tty->p_console->original_addr;

    /* 默认光标位置在最开始处 */
    p_tty->p_console->cursor = p_tty->p_console->original_addr;

    mili_delay(300);
    clear_screen(p_tty->p_console);
    out_string(p_tty->p_console, "    MuteOS  login tty", White);
    out_string(p_tty->p_console, "\n> ", Blue);
    out_string(p_tty->p_console, "$ ", Light_Green);
}


/*======================================================================*
               out_char
 *======================================================================*/
PUBLIC void out_char(CONSOLE* p_con, char ch, int color)
{
    if (ch == '\n') {
        p_con->cursor = ((p_con->cursor - p_con->current_start_addr) / 80 + 1) * 80;
    } else {
        u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);
        *p_vmem++ = ch;
        *p_vmem++ = color;
        p_con->cursor++;
    }

    set_cursor(p_con->cursor);
}


/*======================================================================*
               out_string
 *======================================================================*/
PUBLIC void out_string(CONSOLE* p_con, char* str, int color)
{
    for (char* p = str; p && *p != '\0'; p++) {
        out_char(p_con, *p, color);
    }
}


/*======================================================================*
                set_cursor
 *======================================================================*/
PRIVATE void set_cursor(unsigned int position)
{
    disable_int();
    out_byte(CRTC_ADDR_REG, CURSOR_H);
    out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
    out_byte(CRTC_ADDR_REG, CURSOR_L);
    out_byte(CRTC_DATA_REG, position & 0xFF);
    enable_int();
}


/*======================================================================*
                               clear_screen
 *======================================================================*/
PUBLIC void clear_screen(CONSOLE* p_con)
{
    p_con->cursor = 0;
    for (int i = 0; i <= 25; i++) {
        for (int j = 0; j <= 80; j++) {
            out_string(p_con, " ", White);
        }
    }
    p_con->cursor = 0;
}