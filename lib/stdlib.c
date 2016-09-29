/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            stdlib.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"


PRIVATE int vsprintf(char* buf, const char* fmt, va_list args);

/*======================================================================*
                               itoa
 *======================================================================*/
/* 数字前面的 0 不被显示出来, 比如 0000B800 被显示成 B800 */
PUBLIC char * itoa(char * str, int num)
{
    char *  p = str;
    char ch;
    int i;
    int flag = 0;

    *p++ = '0';
    *p++ = 'x';

    if (num == 0) {
        *p++ = '0';
    }
    else{   
        for (i = 28; i >= 0; i -= 4){
            ch = (num >> i) & 0xF;
            if(flag || (ch > 0)){
                flag = 1;
                ch += '0';
                if(ch > '9'){
                    ch += 7;
                }
                *p++ = ch;
            }
        }
    }

    *p = 0;

    return str;
}


PUBLIC char* itoa10(char* str, int num)
{
    char buf[256];
    char *p = str;
    int i = 0;

    if (num == 0) {
        *buf = '0';
        i = 1;
    }

    for (; num; num /= 10, i++) {
        buf[i] = num % 10 + '0';
    }

    for (i--; i >= 0; i--) {
        *p++ = buf[i];
    }
    *p = '\0';

    return str;
}


PUBLIC int strlen(char* str)
{
    int i = 0;
    while (*str++) {
        i++;
    }
    return i;
}

/*======================================================================*
                               print_bit
 *======================================================================*/
PUBLIC void print_bit(int input, char color)
{
    char output[16];
    itoa(output, input);
    print(output, color);
}


/*======================================================================
                printf
 *======================================================================*/
PUBLIC int printf(const char* fmt, ...)
{
    int i;
    char buf[256];
    va_list arg = (va_list)((char*)(&fmt) + 4);
    i = vsprintf(buf, fmt, arg);
    write(buf, i);

    return i;
}

/*======================================================================
                clear_screen
 *======================================================================*/
PUBLIC void clear()
{
    clscreen();
}

PUBLIC char keyboard_input()
{
    char key = sys_char;
    sys_char = 0;
    return key;
}


/*======================================================================*
                vsprintf
 *======================================================================*/
PRIVATE int vsprintf(char* buf, const char* fmt, va_list args)
{
    char* p;
    char tmp[256];
    va_list p_next_arg = args;

    for (p = buf; *fmt; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }
        fmt++;

        switch (*fmt) {
            case 'x' :
                itoa(tmp, *((int*)p_next_arg));
                strcpy(p, tmp);
                p_next_arg += 4;
                p += strlen(tmp);
                break;
            case 'd' :
                itoa10(tmp, *((int*)p_next_arg));
                strcpy(p, tmp);
                p_next_arg += 4;
                p += strlen(tmp);
                break;
            case 's' :
                strcpy(p, (char*)p_next_arg);
                p += strlen((char*)p_next_arg);
                p_next_arg += 4;
                break;
            default :
                break;
        }
    }
    return p - buf;
}


