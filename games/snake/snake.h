#ifndef SNAKE_H
#define SNAKE_H

#define INIT_LENGTH 3
#define ROW         24
#define COLUMN      80

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

typedef struct{
    int x, y;
} Node;

void init_scr();
void init_snake();
void move();
void game_start();
void draw();
void copy();
int overlap();
void game_over();
void game_restart();
void keyboard();
void generate_food();
void eat();

// keyboard constant
#define UP          0x25   /* Up       */
#define DOWN        0x26   /* Down     */
#define LEFT        0x27   /* Left     */
#define RIGHT       0x28   /* Right    */
#define ENTER       0x03   /* Enter    */

#endif
