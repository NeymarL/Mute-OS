TOOLPATH = tools/
INCPATH  = lib/

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

bootpack.o :
	gcc -c bootpack.c -o bootpack.o

bootpack.elf : bootpack.o
	ld bootpack.o -o bootpack.elf -T $(LDFILE)

bootpack.bin : bootpack.elf
	objcopy -R .pdr -R .comment -R .note -S -O binary bootpack.elf bootpack.bin

Mute-OS.sys : bootpack.bin ipl.bin
	cat ipl.bin bootpack.bin > Mute-OS.sys

img : Mute-OS.sys
	dd if=Mute-OS.sys of=Mute-OS.img

run : img
	qemu-system-i386 -fda Mute-OS.img

clean :
	rm bootpack.o
	rm bootpack.elf
	rm bootpack.bin
	rm Mute-OS.*
	rm ipl.bin
	