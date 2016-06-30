MAKE     = 	make -r
LDFILE   = 	rule.ld
DD 		 = 	dd
NASM     =  nasm
CC 		 =  gcc
QEMU     =  qemu-system-i386


# 生成img镜像
default:
	$(MAKE) img

# 写入到u盘
usb: img
	$(DD) if=Mute-OS.img of=/dev/sdb

ipl.bin : ipl.nas ipl_qemu.nas Makefile
	$(NASM) -f bin ipl.nas -o ipl.bin

asmhead.bin : asmhead.nas
	$(NASM) -f bin asmhead.nas -o asmhead.bin -l asmhead.lst

func.o : func.nas
	$(NASM) -f elf64 func.nas -l func.lst

bootpack.o : bootpack.c
	$(CC) -ggdb -c bootpack.c -o bootpack.o

graphic.o : graphic.c
	$(CC) -ggdb -c graphic.c -o graphic.o

bootpack.elf : bootpack.o func.o graphic.o font.o
	$(LD) bootpack.o func.o graphic.o font.o -o bootpack.elf -T $(LDFILE)

bootpack.bin : bootpack.elf
	objcopy -R .pdr -R .comment -R .note -S -O binary bootpack.elf bootpack.bin

Mute-OS.sys : bootpack.bin ipl.bin asmhead.bin
	cat ipl.bin asmhead.bin bootpack.bin > Mute-OS.sys

font.o : font.c
	$(CC) -c font.c -o font.o
	

img : Mute-OS.sys
	$(DD) if=Mute-OS.sys of=Mute-OS.img

run : ipl_qemu.nas
	$(NASM) -f bin ipl_qemu.nas -o ipl.bin
	$(MAKE) img
	$(QEMU) -drive file=Mute-OS.img,format=raw,index=0,if=floppy

debug : img
	$(QEMU) -s -S -drive file=Mute-OS.img,format=raw,index=0,if=floppy

#Mute-OS.sys : asmhead.bin ipl.bin
#	$(NASM) -o test.o test.asm -l test.lst
#	cat ipl.bin asmhead.bin test.o > Mute-OS.sys

test : font.o testFont.c
	$(CC) testFont.c font.o -o test
	./test

clean :
	rm *.o
	rm bootpack.elf
	rm bootpack.bin
	rm Mute-OS.*
	rm *.bin
	