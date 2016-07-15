# Makefile for boot
# 

MAKE     = 	make -r
LDFILE   = 	rule.ld
DD 		 = 	dd
NASM     =  nasm
ASMFLAG  =  -I include/
CC 		 =  gcc
QEMU     =  qemu-system-i386
RM		 =  rm

TARGET   =  boot.bin loader.bin

RAW 	 =  raw.img
CONTAINER= 	image.img
OSIMG 	 = 	Muteos.img

KERNELDIR= 	../kernel
BOOTDIR	 = 	.
MOUNTDIR =  ../mnt
ROOTDIR  =  ..

everything : ${TARGET}

all : clean everything

boot.bin : boot.asm include/load.inc include/fat12hdr.inc
	$(NASM) $(ASMFLAG) -o $@ $<

loader.bin : loader.asm include/load.inc include/fat12hdr.inc include/pm.inc
	$(NASM) $(ASMFLAG) -o $@ $<

kernel :
	$(NASM) -f elf64 ${KERNELDIR}/kernel.asm -o ${KERNELDIR}/kernel.bin


usb : img
	$(DD) if=${OSIMG} of=/dev/sdb

f12image : bootloader kernel
	$(DD) if=/dev/zero of=${ROOTDIR}/${RAW} bs=1440K count=1
	mkfs.fat -F 12 -r 224 -s 1 -S 512 -M 0xF0 -R 1 ${ROOTDIR}/${RAW}
	mount ${ROOTDIR}/${RAW} ${MOUNTDIR}
	cp loader.bin ${MOUNTDIR}
	cp ${KERNELDIR}/kernel.bin ${MOUNTDIR}
	umount ${ROOTDIR}/${RAW}

img : f12image
	$(DD) if=${ROOTDIR}/${RAW} of=${ROOTDIR}/${CONTAINER} bs=512 skip=1
	cat boot.bin ${ROOTDIR}/${CONTAINER} > ${ROOTDIR}/${OSIMG}

run : img
	$(QEMU) -drive file=${ROOTDIR}/${OSIMG},format=raw,index=0,if=floppy

debug : bootloader
	$(QEMU) -s -S -drive file=${ROOTDIR}/${OSIMG},format=raw,index=0,if=floppy

clean :
	$(RM) -f ${TARGET}
