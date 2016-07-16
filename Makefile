############################################
# Makefile for MuteOS                      #
# 目标 : 生成MuteOS的磁盘镜像 MuteOS.img    #
############################################

# Entry point of OS
ENTRYPOINT = 0x600

# programs, flags
MAKE     = 	make -r
LD   	 = 	ld
DD 		 = 	dd
NASM     =  nasm
CC 		 =  gcc
QEMU     =  qemu-system-i386
RM		 =  rm -f
ASMBFLAGS=  -I boot/include/
ASMKFLAGS=  -I include/ -f elf64
CFLAGS   =  -I include/ -c
LDFLAGS  =  -s -Ttext ${ENTRYPOINT}

# directories
KERNELDIR= 	kernel
BOOTDIR	 = 	boot
MOUNTDIR =  mnt
LIBDIR 	 = 	lib
ROOTDIR  =  .

# this program
OBJS 	 = 	kernel/kernel.o kernel/start.o lib/lib.o kernel/i8259.o kernel/global.o \
			kernel/protect.o lib/stdlib.o
BOOTBINS = 	boot/boot.bin boot/loader.bin
KERNBINS =  kernel/kernel.bin
RAW 	 =  raw.img
CONTAINER= 	image.img
OSIMG 	 = 	MuteOS.img

TARGET   = 	${OSIMG}

everything : ${TARGET}

all : realclean everything

boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	$(NASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/load.inc \
			 boot/include/fat12hdr.inc boot/include/pm.inc
	$(NASM) $(ASMBFLAGS) -o $@ $<

kernel/kernel.bin : ${OBJS}
	$(LD) $(LDFLAGS) -o $@ ${OBJS}

kernel/kernel.o : kernel/kernel.asm
	$(NASM) $(ASMKFLAGS) -o $@ $<

kernel/start.o : kernel/start.c include/const.h include/func.h include/type.h \
			include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o : kernel/i8259.c include/const.h include/func.h include/type.h \
			include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/global.o : kernel/global.c
	$(CC) $(CFLAGS) -o $@ $<

kernel/protect.o : kernel/protect.c
	$(CC) $(CFLAGS) -o $@ $<

lib/lib.o : lib/lib.asm
	$(NASM) $(ASMKFLAGS) -o $@ $<

lib/stdlib.o : lib/stdlib.c
	$(CC) $(CFLAGS) -o $@ $<

usb : $(OSIMG)
	$(DD) if=${OSIMG} of=/dev/sdb

${RAW} : boot/loader.bin  kernel/kernel.bin
	$(DD) if=/dev/zero of=${RAW} bs=1440K count=1
	mkfs.fat -F 12 -r 224 -s 1 -S 512 -M 0xF0 -R 1 ${RAW}
	mount ${RAW} ${MOUNTDIR}
	cp ${BOOTDIR}/loader.bin ${MOUNTDIR}
	cp ${KERNELDIR}/kernel.bin ${MOUNTDIR}
	umount ${RAW}

${OSIMG} : ${RAW} ${BOOTDIR}/boot.bin
	$(DD) if=${RAW} of=${CONTAINER} bs=512 skip=1
	cat ${BOOTDIR}/boot.bin ${CONTAINER} > ${OSIMG}

run : ${OSIMG} 
	$(QEMU) -drive file=${OSIMG},format=raw,index=0,if=floppy

debug : ${OSIMG}
	$(QEMU) -s -S -drive file=${OSIMG},format=raw,index=0,if=floppy

clean :
	$(RM)  ${OBJS} ${RAW} ${CONTAINER}

realclean :
	$(RM) ${TARGET} ${OBJS} ${BOOTBINS} ${KERNBINS} ${RAW} ${CONTAINER}

