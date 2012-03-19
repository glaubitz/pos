# pOS
# main Makefile
# 11/26/03

ARCH= c64
AS= ca65
ASFLAGS=
CC= cc65
CCFLAGS=
LD= ld65
LDFLAGS= -tc64

all: boot kernel pos.lib

kernel: entry.o kernel.o drivers.o fs.o lib.o
	$(LD) $(LDFLAGS) arch/$(ARCH)/entry.o \
	arch/$(ARCH)/nmi.o arch/$(ARCH)/irq.o \
	kernel/kernel.o \
	drivers/drivers.o \
	vfs/vfs.o \
	lib/lib.o

tools:
	make -C tools

boot entry.o irq.o nmi.o:
	make -C arch/$(ARCH)

pos.lib: kernel
	make -C lib pos.lib

kernel.o:
	make -C kernel

drivers.o:
	make -C drivers

vfs.o:
	make -C vfs

lib.o:
	make -C lib

tkconfig:
	tools/tkconfig.tk

clean:
	make -C arch/$(ARCH) clean
	make -C kernel clean
	make -C drivers clean
	make -C tools clean
	make -C lib clean
	make -C vfs clean
