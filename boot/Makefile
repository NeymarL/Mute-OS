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

default :
	$(MAKE) run

bin : boot.asm loader.asm
	$(NASM) boot.asm -o boot.bin
	$(NASM) loader.asm -o loader.bin

boot.o : boot.asm
	$(NASM) boot.asm -o boot.o

usb : img
	$(DD) if=a.img of=/dev/sdb

f12image : bin
	$(DD) if=/dev/zero of=${RAW} bs=1440K count=1
	mkfs.fat -F 12 -r 224 -s 1 -S 512 -M 0xF0 -R 1 ${RAW}
	mount ${RAW} test
	cp loader.bin test/
	umount ${RAW}

img : bin f12image
	$(DD) if=${RAW} of=${CONTAINER} bs=512 skip=1
	cat boot.bin ${CONTAINER} > ${OSIMG}

run : img
	$(QEMU) -drive file=${OSIMG},format=raw,index=0,if=floppy

debug : bin
	$(QEMU) -s -S -drive file=${OSIMG},format=raw,index=0,if=floppy

clean :
	$(RM) *.bin
	$(RM) *.img
	$(RM) *.o