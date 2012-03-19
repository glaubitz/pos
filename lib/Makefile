# pOS Makefile
# 07/23/03

ARCH= c64
AS= ca65
ASFLAGS= -tc64
CC= cc65
CCFLAGS= -Iinclude -tc64
LD= ld65
LDFLAGS= -tc64

.c.o:
	$(CC) $(CCFLAGS) -o $*.o $<

pos.lib: pos.o
	ar65 a pos.lib pos.o
