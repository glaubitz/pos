# pOS
# C64 specific code makefile
# 07/23/03

AS= ca65
ASFLAGS= -tc64
CC= cc65
CCFLAGS= -I../../include -tc64
LD= ld65
LDFLAGS= -tc64

all: boot entry.o irq.o nmi.o

.s.o:
	$(AS) $(ASFLAGS) -o $*.o $<

boot: boot.o
	$(LD) $(LDFLAGS) boot.o -o boot

clean:
	rm -f *.o
	rm -f boot
