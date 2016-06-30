/*
 * kernel.h
 */

#define VRAM            0xa0000         // vedio ram

typedef struct{
    char cyls, leds, vmods, reserve;
    short scrnx, scrny;
    char* vram;
} BOOTINFO;



// assembly functions
void io_hlt();                          // halt
void io_cli();                          // clear interrupt flag
void io_out8(int port, int data);
int io_load_eflags();
void io_store_eflags(int eflags);
void write_mem8(int addr, int data);    // write data to address addr

// C functions 

