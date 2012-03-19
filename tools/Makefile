# pOS tools MakeFile
# 07/23/03

CC= gcc
CCFLAGS= -I../include
LD= ld
LDFLAGS=


all : config makedev

config : config.c
	$(CC) $(CCFLAGS) -o config.o config.c
	$(LD) $(LDFLAGS) -o config config.o

makedev : makedev.c
	$(CC) $(CCFLAGS) -o makedev.o makedev.c
	$(LD) $(LDFLAGS) -o makedev makedev.o

clean:
	rm -f *.o
	rm -f config
	rm -f makedev
