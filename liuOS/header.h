#ifndef header
#define header

#include "x86.h"

#define io_halt() asm("hlt")
#define write_mem8(addr,data8)   (*(volatile char *)(addr))=(char)data8
#define io_cli()  asm("cli")
#define io_sti()  asm("sti")
#define io_stihlt() (io_cli();io_halt;)

/*
extern void clear_screen(char color) ; //color=15 pure white color=40 red
extern void color_screen(char color) ;
extern void init_palette(void);//用现成的table_rgb来初始化调色板
extern void set_palette(int start,int end, unsigned char *rgb);
extern void boxfill8(unsigned char *vram,int xsize,unsigned char color,int x0,int y0,int x1,int y1);
extern void boxfill(unsigned char color,int x0,int y0,int x1,int y1);
*/

#endif