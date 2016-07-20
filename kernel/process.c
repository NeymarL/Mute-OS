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
