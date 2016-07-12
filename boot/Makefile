MAKE     = 	make -r
LDFILE   = 	rule.ld
DD 		 = 	dd
NASM     =  nasm
CC 		 =  gcc
QEMU     =  qemu-system-i386
RM		 =  rm

RAW 	 =  raw.img
CONTAINER= 	image.img
OSIMG 	 = 	Muteos.img

KERNELDIR= 	../kernel
BOOTDIR	 = 	.
MOUNTDIR =  ../mnt
ROOTDIR  =  ..

default :
	$(MAKE) bootloader

bootloader : boot.asm loader.asm
	$(NASM) boot.asm -o boot.bin
	$(NASM) loader.asm -o loader.bin

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
	$(RM) *.bin
	$(RM) *.img
