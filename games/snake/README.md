# Snakes Gmae

## Start

The start code is at function `TestB`  in  `kernel/main.c`, something like this:

```C
void TestB()
{
    mili_delay(4000);
    printf("Game Start!");
    mili_delay(1000);
    clear();
    disable_tty_output();
    game_start();
}
```

If you want to play the game, modify `TestB` that make it identify to the upper one.

If you do not want to run the game, just replace the code in `TestB` with a `while(1){}` loop.

## Game Control

You can use the UP arrow, the DOWN arrow, the LEFT arrow and the RIGHT arrow to control the snake's moviong direction. When you die, you can press ENTER to restart.

## Screenshot

![Snake1](games/snake/snake1.png)

![Snake2](games/snake/snake2.png)



