/*
 * config.h
 * main configuration file
 * 12/19/04
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <devices.h>

/* basic configuration */

#define MACHINE	C64
#include 	"../arch/MACHINE/MACHINE.h"
#define ASM	asm-MACHINE
#else

#ifndef MACHINE
#error				No machine type defined
#endif

#ifndef				__16BITMEM
#else
#ifdef				__32BITMEM
#else
#error				No memory type defined
#endif
#endif

#define c_ROOTDEVMAJ 0
#define c_ROOTDEVMIN 0
#define c_ROOTDEVFILE  "/rootdev"
#define c_ROOTDEVSTRUCT {&k_kernel.driverlist[c_ROOTDEVMAJ],c_ROOTDEVMIN}

/* device configuration */

/* filesystems */

#define c_FS1541FS

/* initial mountlist */
/*
#define c_mountlist \
{ \
\
	{"/", "/dev/block0", c_PFS}\
}
*/ // outdated !
#endif
