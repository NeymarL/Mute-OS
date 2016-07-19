/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"

PUBLIC  PROCESS         proc_table[NR_TASKS];

PUBLIC  char            task_stack[STACK_SIZE_TOTAL];
