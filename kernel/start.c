/**
 * start.c
 */

#include "../include/type.h"
#include "../include/const.h"
#include "../include/func.h"

PUBLIC  u8          gdt_ptr[6];     /* 0~15:Limit  16~47:Base */
PUBLIC  DESCRIPTOR  gdt[GDT_SIZE];

PUBLIC void cstart()
{
    print("------------Welcom to use MuteOS !!!-------------");
}
