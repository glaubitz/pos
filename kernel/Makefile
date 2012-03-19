# pOS
# Kernel Makefile
# 01/15/05

AS= ca65
ASFLAGS= -tc64
CC= cc65
CCFLAGS= -I../include -tc64
LD= ld65
LDFLAGS= -tc64

OBJS= console.o device.o memory.o panic.o process.o schedule.o \
                signal.o sysinit.o time.o version.o

all: kernel.o

.c.o:
	$(CC) $(CCFLAGS) -o $*.o $<

kernel.o: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.o

clean:
	rm -f *.o
