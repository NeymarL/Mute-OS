MAKE     = 	make -r
LDFILE   = 	rule.ld
DD 		 = 	dd
NASM     =  nasm
CC 		 =  gcc
QEMU     =  qemu-system-i386
RM		 =  rm

default :
	$(MAKE) run

bin : boot.asm
	$(NASM) boot.asm -o boot.bin

boot.o : boot.asm
	$(NASM) boot.asm -o boot.o

usb : img
	$(DD) if=a.img of=/dev/sdb

img : bin
	$(DD) if=boot.bin of=a.img bs=512 count=1 conv=notrunc

run : img
	$(QEMU) -drive file=a.img,format=raw,index=0,if=floppy

debug : bin
	$(QEMU) -s -S -drive file=boot.bin,format=raw,index=0,if=floppy

clean :
	$(RM) *.bin
	$(RM) *.img
	$(RM) *.o