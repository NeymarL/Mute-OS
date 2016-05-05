#include "header.h"

void clear_screen(char color) //15:pure white
{
  int i;
  for(i=0xa0000;i<0xaffff;i++)
  {
  write_mem8(i,color);  //if we write 15 ,all pixels color will be white,15 mens pure white ,so the screen changes into white

  }
}

void color_screen(char color) //15:pure white
{
  int i;
  color=color;
  for(i=0xa0000;i<0xaffff;i++)
  {
  write_mem8(i,i);  //if we write 15 ,all pixels color will be white,15 mens pure white ,so the screen changes into white

  }
}


void init_palette(void)
{
  //16种color，每个color三个字节。
  static unsigned char table_rgb[16*3]=
  {
    0x00,0x00,0x00,   /*0:black*/
    0xff,0x00,0x00,   /*1:light red*/ 
    0x00,0xff,0x00,   /*2:light green*/   
    0xff,0xff,0x00,   /*3:light yellow*/
    
    0x00,0x00,0xff,   /*4:light blue*/
    0xff,0x00,0xff,   /*5:light purper*/ 
    0x00,0xff,0xff,   /*6:light blue*/
    0xff,0xff,0xff,   /*7:white*/
 
    0xc6,0xc6,0xc6,   /*8:light gray*/
    0x84,0x00,0x00,   /*9:dark red*/
    0x00,0x84,0x00,   /*10:dark green*/ 
    0x84,0x84,0x00,   /*11:dark yellow*/
   
    0x00,0x00,0x84,   /*12:dark 青*/
    0x84,0x00,0x84,   /*13:dark purper*/
    0x00,0x84,0x84,   /*14:light blue*/
    0x84,0x84,0x84,   /*15:dark gray*/
  };
    set_palette(0,15,table_rgb);
    return;
    
  
}

//设置调色板，  只用到了16个color,后面的都没有用到。
void set_palette(int start,int end, unsigned char *rgb)
{
  int i,eflag;
  eflag=read_eflags();   //记录从前的cpsr值
 
  io_cli(); // disable interrupt

  outb(0x03c8,start);
  for(i=start;i<=end;i++)
  {
    outb(0x03c9,rgb[0]);    
    outb(0x03c9,rgb[1]);   
    outb(0x03c9,rgb[2]);   
    rgb=rgb+3;
  }
  
write_eflags(eflag);  //恢复从前的cpsr
  return;
  
}

void boxfill8(unsigned char *vram,int xsize,unsigned char color,int x0,int y0,int x1,int y1)
{
 int x,y;
 for(y=y0;y<=y1;y++)
 {
   for(x=x0;x<=x1;x++)
   {
      vram[y*xsize+x]=color;
   }
 }
   
}
void boxfill(unsigned char color,int x0,int y0,int x1,int y1)
{
  boxfill8((unsigned char *)0xa0000,320,color,x0,y0,x1,y1);
}