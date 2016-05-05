/*****************/
/*     main.c    */
/*****************/

#include "header.h"


int main()
{
    int i;
    // MOV  DWORD [0x0ff8], 0x000a0000
    // movl $0x000a0000, $0x0ff8
    __asm__ volatile(
        "movl $0x000a0000, %ebx\n\t"\
        "movl $0x0ff8, %edi\n\t"\
        "movl %ebx, (%edi)"
    );     

    for (i = 0xa0000; i <= 0xaffff; i++){
        write_mem8(i, (char)(i & 0x0f));
        //p = (char*)i;
        //*p = i & 0x0f;
    }
    for (;;){
        io_halt();
    }
    return 0;
}
