/*****************/
/*     main.c    */
/*****************/

#define COL8_000000     0
#define COL8_FF0000     1
#define COL8_00FF00     2
#define COL8_FFFF00     3
#define COL8_0000FF     4
#define COL8_FF00FF     5
#define COL8_00FFFF     6
#define COL8_FFFFFF     7
#define COL8_C6C6C6     8
#define COL8_840000     9
#define COL8_008400     10
#define COL8_848400     11
#define COL8_000084     12
#define COL8_840084     13
#define COL8_008484     14
#define COL8_848484     15

#define VRAM            0xa0000         // vedio ram
#define XSIZE           320
#define YSIZE           200

// assembly functions
void io_hlt();                          // halt
void io_cli();                          // clear interrupt flag
void io_out8(int port, int data);
int io_load_eflags();
void io_store_eflags(int eflags);
void write_mem8(int addr, int data);    // write data to address addr

// C functions 
void init_palette();
void set_palette(int start, int end, unsigned char* rgb);
void draw_rect(int color, int x0, int y0, int x1, int y1);

int main()
{
    init_palette();     // 设定调色板

    draw_rect(COL8_008484,  0, 0,          XSIZE -  1, YSIZE - 29);
    draw_rect(COL8_C6C6C6,  0, YSIZE - 28, XSIZE -  1, YSIZE - 28);
    draw_rect(COL8_FFFFFF,  0, YSIZE - 27, XSIZE -  1, YSIZE - 27);
    draw_rect(COL8_C6C6C6,  0, YSIZE - 26, XSIZE -  1, YSIZE -  1);

    draw_rect(COL8_FFFFFF,  3, YSIZE - 24, 59, YSIZE - 24);
    draw_rect(COL8_FFFFFF,  2, YSIZE - 24,  2, YSIZE -  4);
    draw_rect(COL8_848484,  3, YSIZE -  4, 59, YSIZE -  4);
    draw_rect(COL8_848484, 59, YSIZE - 23, 59, YSIZE -  5);
    draw_rect(COL8_000000,  2, YSIZE -  3, 59, YSIZE -  3);
    draw_rect(COL8_000000, 60, YSIZE - 24, 60, YSIZE -  3);

    draw_rect(COL8_848484, XSIZE - 47, YSIZE - 24, XSIZE -  4, YSIZE - 24);
    draw_rect(COL8_848484, XSIZE - 47, YSIZE - 23, XSIZE - 47, YSIZE -  4);
    draw_rect(COL8_FFFFFF, XSIZE - 47, YSIZE -  3, XSIZE -  4, YSIZE -  3);
    draw_rect(COL8_FFFFFF, XSIZE -  3, YSIZE - 24, XSIZE -  3, YSIZE -  3);

    while(1){
        io_hlt();
    }
    return 0;
}


void draw_rect(int color, int x0, int y0, int x1, int y1)
{
    int vram = VRAM;
    int xsize = 320;
    for (int y = y0; y <= y1; y++){
        for (int x = x0; x <= x1; x++){
            //vram[y * xsize + x] = color;
            write_mem8(vram + x + y * xsize, color);
        }
    }
}

void init_palette()
{
    static unsigned char table_rgb[16 * 3] = {  
        0x00, 0x00, 0x00,   // 0. black
        0xff, 0x00, 0x00,   // 1. light red
        0x00, 0xff, 0x00,   // 2. light green
        0xff, 0xff, 0x00,   // 3. light yellow
        0x00, 0x00, 0xff,   // 4. light blue
        0xff, 0x00, 0xff,   // 5. light purple
        0x00, 0xff, 0xff,   // 6. shadow light blue
        0xff, 0xff, 0xff,   // 7. white
        0xc6, 0xc6, 0xc6,   // 8. light grey
        0x84, 0x00, 0x00,   // 9. dark red
        0x00, 0x84, 0x00,   // 10. dark green
        0x84, 0x84, 0x00,   // 11. dark yellow
        0x00, 0x00, 0x84,   // 12. dark qing
        0x84, 0x00, 0x84,   // 13. dark purple
        0x00, 0x84, 0x84,   // 14. shadow dark blue
        0x84, 0x84, 0x84    // 15. dark grey
    };                      // static char 相当于汇编中的 DB
    set_palette(0, 15, table_rgb);
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = io_load_eflags();  // 记录中断许可标志的值
    io_cli();                   // 将中断许可标志置0,禁止中断
    io_out8(0x03c8, start);     // 将想要设定的调色板号码写入 0x03c8
    for (i = start; i <= end; i++){
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);    // 恢复中断许可标志
}
