#include <stdio.h>

extern char FONTS[0xff + 1][16 * 8 + 1];        // fonts

void draw_font(char c)
{
    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 8; j++){
            if (FONTS[c][j + i * 8] == '*'){
                printf("%c", FONTS[c][j + i * 8]);
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void draw_str(char* str)
{
    while (*str != '\0'){
        draw_font(*str);
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

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = 0;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = 1;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = backgroung_color;
            }
        }
    }
}

int main()
{
    //draw_str("12");
    //draw_font('F');
    char mcursor[256];
    init_mouse_cursor8(mcursor, 2);
    for (int i = 0; i < 256; ++i)
    {
        printf("%d", mcursor[i]);
        if(i != 0 && (i + 1) % 16 == 0){
            printf("\n");
        }
    }
    return 0;
}

