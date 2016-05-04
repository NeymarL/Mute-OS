# ÉâÉCÉuÉâÉäê∂ê¨ópMakefile

TARGET   = harilibc.lib
OBJS     = startup.obj

TOOLPATH = ../tools/
INCPATH  = ./

SJISCONV = $(TOOLPATH)sjisconv -s
CC1      = gcc
GAS2NASK = $(TOOLPATH)gas2nask -a
NASK     = $(TOOLPATH)nask
GOLIB    = $(TOOLPATH)golib00
MAKE     = make -r
DELE     = del

ALL :
	$(MAKE) $(TARGET)

%.gas : %.ca Makefile
	$(CC1) -S -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj

$(TARGET) : $(OBJS) Makefile
	$(GOLIB) out:$(TARGET) $(OBJS)

clean :
	-$(DELE) *.obj

src_only :
	$(MAKE) clean
	-$(DELE) $(TARGET)
