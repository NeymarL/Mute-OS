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
PRIVATE void set_video_start_addr(u32 addr);
PRIVATE void flush(CONSOLE* p_con);


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

    if (nr_tty == 0) {
        p_tty->p_console->cursor = p_tty->p_console->original_addr + 80 * 9;
    } 

    out_string(p_tty->p_console, "\tMuteOS   login    tty", White);
    out_char(p_tty->p_console, nr_tty + '0', White);
    out_string(p_tty->p_console, "\n> ", Blue);
    out_string(p_tty->p_console, "$ ", Light_Green);
}


/*======================================================================*
               select_console
 *======================================================================*/
PUBLIC void select_console(int nr_console)
{
    if ((nr_console <0) || (nr_console >= NR_CONSOLES)) {
        return;
    }
    nr_current_console = nr_console;

    set_video_start_addr(console_table[nr_console].current_start_addr);
    set_cursor(console_table[nr_console].cursor);
}


/*======================================================================*
               out_char
 *======================================================================*/
PUBLIC void out_char(CONSOLE* p_con, char ch, int color)
{
    int row, column;

    u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);

    switch(ch) {
        case '\n':
            if (p_con->cursor < p_con->original_addr + 
                p_con->v_mem_limit - SCREEN_WIDTH) {
                p_con->cursor = p_con->original_addr + SCREEN_WIDTH * 
                        ((p_con->cursor - p_con->original_addr) / SCREEN_WIDTH + 1);
            }
            break;
        case '\b':
            row = (p_con->cursor - p_con->original_addr) / SCREEN_WIDTH;
            column = p_con->cursor - row * SCREEN_WIDTH;
            if (column > 4) {
                p_con->cursor--;
                *(p_vmem - 2) = ' ';
                *(p_vmem - 1) = White;
            }
            break;
        case '\t':
            out_string(p_con, "    ", color);
            break;
        default:
            if (p_con->cursor < p_con->original_addr + p_con->v_mem_limit - 1) {
                *p_vmem++ = ch;
                *p_vmem++ = color;
                p_con->cursor++;
            }
            break;
    }

    while (p_con->cursor >= p_con->current_start_addr + SCREEN_SIZE) {
        scroll_screen(p_con, SCR_DN);
    }
    if (p_con == console_table + nr_current_console){
        flush(p_con);
    }
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


/*======================================================================
                scroll_screen
 *======================================================================*/
PUBLIC void scroll_screen(CONSOLE* p_con, int direction)
{
    if (direction == SCR_UP) {
        if (p_con->current_start_addr > p_con->original_addr) {
            p_con->current_start_addr -= SCREEN_WIDTH;
        }
    } else if (direction == SCR_DN) {
        if (p_con->current_start_addr + SCREEN_SIZE < 
            p_con->original_addr + p_con->v_mem_limit) {
            p_con->current_start_addr += SCREEN_WIDTH;
        }
    }

    set_video_start_addr(p_con->current_start_addr);
    set_cursor(p_con->cursor);
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


PRIVATE void set_video_start_addr(u32 addr)
{
    disable_int();
    out_byte(CRTC_ADDR_REG, START_ADDR_H);
    out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
    out_byte(CRTC_ADDR_REG, START_ADDR_L);
    out_byte(CRTC_DATA_REG, addr & 0xFF);
    enable_int();
}


PRIVATE void flush(CONSOLE* p_con) 
{
    set_cursor(p_con->cursor);
    set_video_start_addr(p_con->current_start_addr);
}
