/*****************/
/*     main.c    */
/*****************/

//#include "header.h"

void test();
void io_hlt();
void write_mem8(int addr, int data);

int main()
{
    int i;   
    //char *p;
    for (i = 0xa0000; i <= 0xaffff; i++){
        write_mem8(i, 14);
        //p = (char*)i;
        //*p = 14;
    }
    //test();
    while(1){
        io_hlt();
    }
    return 0;
}
