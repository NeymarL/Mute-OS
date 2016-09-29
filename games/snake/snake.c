#include "snake.h"
#include "../../lib/lib.h"

char screen[ROW][COLUMN + 1];
int direction = Right;
Node snake[ROW * COLUMN];
int head = 0;
int gameover = 1;
Node food = {-1, -1};

void init_scr()
{
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COLUMN + 1; j++) {
            screen[i][j] = ' ';
            if (j == COLUMN) {
                screen[i][j] = '\0';
            }
        }
    }
}

void init_snake()
{
    direction = Right;
    for (int i = 0; i < ROW * COLUMN; i++) {
        snake[i].x = snake[i].y = -1;
    }
    for (int i = 0; i < INIT_LENGTH; i++) {
        snake[i].x = i;
        snake[i].y = 0;
        head = i;
    }
    copy();
}

void copy()
{
    init_scr();
    for (int i = head; i >= 0; i--) {
        screen[snake[i].y][snake[i].x] = '*';
    }
    screen[food.y][food.x] = '*';
}

void draw()
{
    for (int i = 0; i < ROW; i++) {
        printf(screen[i]);
    }
}


void move()
{
    Node next;
    if (direction == Left) {
        next.x = snake[head].x - 1;
        next.y = snake[head].y;
    } else if (direction == Right) {
        next.x = snake[head].x + 1;
        next.y = snake[head].y;
    } else if (direction == Up) {
        next.y = snake[head].y - 1;
        next.x = snake[head].x;
    } else {
        next.y = snake[head].y + 1;
        next.x = snake[head].x;
    }
    if (next.x == food.x && next.y == food.y) {
        eat();
        generate_food();
    }
    else {
        for (int i = 0; i < head; i++) {
            snake[i] = snake[i + 1];
        }
        snake[head] = next;
    }
    
    if (snake[head].x < 0 || snake[head].x > COLUMN || snake[head].y < 0 ||
        snake[head].y > ROW || overlap(snake[head])) {
        game_over();
    }
    copy();
}

int overlap(Node node)
{
    for (int i = 0; i < head; i++) {
        if (snake[i].x == node.x && 
            snake[i].y == node.y) {
            return 1;
        }
    }
    return 0;
}

void game_over()
{
    gameover = 1;
    printf("Game Over!!\tPress Enter to Restart!");
}

void game_restart()
{
    clear();
    init_snake();
    generate_food();
    gameover = 0;
}

void eat()
{
    snake[++head] = food;
}

void keyboard()
{
    char key = keyboard_input();
    switch(key) {
        case UP:
            if (direction != Down) {
                direction = Up; 
            }
            break;
        case DOWN:
            if (direction != Up) {
                direction = Down;
            } 
            break;
        case LEFT:
            if (direction != Right) {
                direction = Left;
            }
            break;
        case RIGHT:
            if (direction != Left) {
                direction = Right;
            }
            break;
        case ENTER:
            if (gameover) {
                game_restart();
            }
            break;
        default:
            break;
    }
}

void generate_food()
{
    do {
        int x = random(COLUMN);
        int y = random(ROW);
        food.x = x;
        food.y = y;
    } while (overlap(food));
    screen[food.y][food.x] = '*';
}


void game_start()
{
    init_snake();
    generate_food();
    gameover = 0;
    while(1){
        if (!gameover) {
            clear();
            draw();
            move();
        }
        //printf("%x ", keyboard_input());
        keyboard();
        sleep(500);
    }
}
