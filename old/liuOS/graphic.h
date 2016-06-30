/*
 * screen.h
 */


#define XSIZE 320
#define YSIZE 200

#define COL8_BLACK      0   // black
#define COL8_BLUE       1   // blue
#define COL8_GREEN      2   // green
#define COL8_L_BLUE     3   // light blue
#define COL8_RED        4   // red
#define COL8_PURPLE     5   // purple
#define COL8_ORANGE     6   // orange
#define COL8_GREY       7   // grey
#define COL8_D_GREY     8   // dark grey
#define COL8_OCBLUE     9   // ocean blue
#define COL8_L_GREN     10  // light green
#define COL8_LLBLUE     11  // light blue
#define COL8_D_PINK     12  // dark pink
#define COL8_L_PURP     13  // light purple
#define COL8_YELLOW     14  // yellow
#define COL8_WHITE      15  // white

extern char FONTS[0xff + 1][16 * 8 + 1];        // fonts

// functions
extern void init_palette();
extern void set_palette(int start, int end, unsigned char* rgb);
extern void draw_rect(int color, int x0, int y0, int x1, int y1);
extern void draw_font(int x, int y, int color, char c);
extern void draw_string(char* str, int x, int y, int color);
extern void init_screen();
extern void init_mouse_cursor8(char* mouse, int backgroung_color);
extern void draw_cursor(int px0, int py0, char *buf);

