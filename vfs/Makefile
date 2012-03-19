# pOS
# Virtual Filesystem Makefile
# 01/15/05

AS= ca65
ASFLAGS= -tc64
CC= cc65
CCFLAGS= -I../include -tc64
LD= ld65
LDFLAGS= -tc64

OBJS= dcache.o filesystem.o file.o mount.o

all: vfs.o

.c.o:
	$(CC) $(CCFLAGS) -o $*.o $<

vfs.o: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o vfs.o

clean:
	rm -f *.o
