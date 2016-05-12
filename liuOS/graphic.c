/*
 * graphic.c
 */

#include "kernel.h"
#include "graphic.h"

void draw_rect(int color, int x0, int y0, int x1, int y1)
{
    int vram = VRAM;
    int xsize = XSIZE;
    for (int y = y0; y <= y1; y++){
        for (int x = x0; x <= x1; x++){
            //vram[y * xsize + x] = color;
            write_mem8(vram + x + y * xsize, color);
        }
    }
}

void init_screen()
{
    draw_rect(COL8_OCBLUE,  0, 0,          XSIZE , YSIZE);
    draw_rect(COL8_GREY,  50, YSIZE - 20, XSIZE -  50, YSIZE);
    draw_rect(COL8_D_GREY,  51, YSIZE - 19, XSIZE -  51, YSIZE);
}

void draw_string(char* str, int x, int y, int color)
{
    while (*str != '\0'){
        draw_font(x, y, color, *str);
        x += 8;
        str++;
    }
}

void init_mouse_cursor8(char* mouse, int backgroung_color)
{
    static char cursor[16][16] = {
        "**************..",
        "*OOOOOOOOOOOOOO*",
        "*OOOOOOOOOOOOO*.",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OO*...*OOO*....",
        "*O*.....*OOO*...",
        "*........*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"
    };
    int x, y;
    int vxsize = XSIZE;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = COL8_BLACK;
                write_mem8(VRAM + y * vxsize + x, COL8_BLACK);
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = COL8_WHITE;
                write_mem8(VRAM + y * vxsize + x, COL8_WHITE);
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = backgroung_color;
                write_mem8(VRAM + y * vxsize + x, COL8_GREEN);
            }
        }
    }
}

void draw_cursor(int px0, int py0, char *buf)
{
    int x, y;
    int pysize = 16;
    int pxsize = 16;
    int vxsize = XSIZE;
    int bxsize = pxsize;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            write_mem8(VRAM + (py0 + y) * vxsize + (px0 + x), 
                buf[y * bxsize + x]);
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
        io_out8(0x03c9, rgb[0] / 16);
        io_out8(0x03c9, rgb[1] / 16);
        io_out8(0x03c9, rgb[2] / 16);
        rgb += 3;
    }
    io_store_eflags(eflags);    // 恢复中断许可标志
}


void draw_font(int x, int y, int color, char c)
{
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 7; j++){
            switch (c) {
                case 'M':
                    if (FONTS['M' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'u':
                    if (FONTS['u' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 't':
                    if (FONTS['t' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'e':
                    if (FONTS['e' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                /*case 'O':
                    if (FONTS['O' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case '6':
                    if (FONTS['6' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case '7':
                    if (FONTS['7' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case '8':
                    if (FONTS['8' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case '9':
                    if (FONTS['9' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case '0':
                    if (FONTS['0' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'a':
                    if (FONTS['a' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'b':
                    if (FONTS['b' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'c':
                    if (FONTS['c' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'd':
                    if (FONTS['d' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'e':
                    if (FONTS['e' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'f':
                    if (FONTS['f' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'g':
                    if (FONTS['g' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'h':
                    if (FONTS['h' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'i':
                    if (FONTS['i' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'j':
                    if (FONTS['j' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'k':
                    if (FONTS['k' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'l':
                    if (FONTS['l' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'm':
                    if (FONTS['m' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'n':
                    if (FONTS['n' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;
                case 'o':
                    if (FONTS['o' + 2][j + i * 8] == '*'){
                        write_mem8(VRAM + (y + i) * XSIZE + x + j, color);
                    }
                    break;*/
                default :
                    break;
            }
        }
    }
}