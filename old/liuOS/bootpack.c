/*****************/
/*     main.c    */
/*****************/

#include "kernel.h"
#include "graphic.h"

int main()
{
    char mcursor[256];

    init_screen();

    draw_font(140, 80, COL8_WHITE, 'M');
    draw_font(150, 80, COL8_WHITE, 'u');
    draw_font(160, 80, COL8_WHITE, 't');
    draw_font(170, 80, COL8_WHITE, 'e');
    //draw_string("Mute", 1, 1, COL8_WHITE);
    init_mouse_cursor8(mcursor, COL8_OCBLUE);
    //draw_cursor(XSIZE / 2, YSIZE / 2, mcursor);

    while(1){
        io_hlt();
    }
    return 0;
}


