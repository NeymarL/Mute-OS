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

usb : bin
	$(DD) if=boot.bin of=/dev/sdb

run : bin
	$(QEMU) -drive file=boot.bin,format=raw,index=0,if=floppy

debug : bin
	$(QEMU) -s -S -drive file=boot.bin,format=raw,index=0,if=floppy

clean :
	$(RM) *.o
	$(RM) *.bin