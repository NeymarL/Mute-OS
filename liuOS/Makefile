TOOLPATH = tools/
INCPATH  = lib/

MAKE     = make -r
NASK     = $(TOOLPATH)nask
GAS2NASK = $(TOOLPATH)gas2nask -a
OBJ2BIM  = $(TOOLPATH)obj2bim
BIM2HRB  = $(TOOLPATH)bim2hrb
RULEFILE = $(INCPATH)haribote.rul


# 生成img镜像
default:
	$(MAKE) img

# 写入到u盘
usb:
	dd if=Mute-OS.img of=/dev/disk1

ipl.bin : ipl.nas Makefile
	nasm ipl.nas -o ipl.bin -l ipl.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

#bootpack.gas : bootpack.c Makefile
#	gcc -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas

#bootpack.o : bootpack.nas Makefile
#	$(NASK) bootpack.nas bootpack.o bootpack.lst

bootpack.o :
	gcc -c bootpack.c -o bootpack.o

naskfunc.o : func.nas Makefile
	$(NASK) func.nas func.o func.lst

bootpack.bim : bootpack.o func.o Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.o func.o

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > haribote.sys


img : ipl.bin haribote.sys Makefile
	cat ipl.bin haribote.sys > Mute-OS.img