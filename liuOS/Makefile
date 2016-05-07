MAKE     = make -r
LDFILE = rule.ld


# 生成img镜像
default:
	$(MAKE) img

# 写入到u盘
usb:
	dd if=Mute-OS.img of=/dev/sdb

ipl.bin : ipl.nas Makefile
	nasm ipl.nas -o ipl.bin

asmhead.bin : asmhead.nas
	nasm asmhead.nas -o asmhead.bin -l asmhead.lst

func.o : func.nas
	nasm -f elf64 func.nas -l func.lst

bootpack.o : bootpack.c
	gcc -ggdb -c bootpack.c -o bootpack.o

bootpack.elf : bootpack.o func.o
	ld bootpack.o func.o -o bootpack.elf -T $(LDFILE)

bootpack.bin : bootpack.elf
	objcopy -R .pdr -R .comment -R .note -S -O binary bootpack.elf bootpack.bin

Mute-OS.sys : bootpack.bin ipl.bin asmhead.bin
	cat ipl.bin asmhead.bin bootpack.bin > Mute-OS.sys

img : Mute-OS.sys
	dd if=Mute-OS.sys of=Mute-OS.img

run : img
	qemu-system-i386 -fda Mute-OS.img

debug : img
	qemu-system-i386 -s -S -fda Mute-OS.img

#Mute-OS.sys : asmhead.bin ipl.bin
#	nasm -o test.o test.asm -l test.lst
#	cat ipl.bin asmhead.bin test.o > Mute-OS.sys


clean :
	rm func.o
	rm bootpack.o
	rm bootpack.elf
	rm bootpack.bin
	rm Mute-OS.*
	rm ipl.bin
	rm asmhead.bin
	
