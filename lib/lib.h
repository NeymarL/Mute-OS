#ifndef _MUTEOS_LIB_H_
#define _MUTEOS_LIB_H_

/* lib/stdlib.c */
char* itoa(char * str, int num);
void print_bit(int input, char color);
int printf(const char* fmt, ...);
char* itoa10(char* str, int num);
int strlen(char* str);
void clear();
char keyboard_input();
void sleep(int milli_sec);
int random(int max);

#endif
