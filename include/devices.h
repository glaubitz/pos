/*
* pOS
* devices.h
* contains all device definitions
*/

// TO BE CORRECTED !

#define 64CON { \
	"64CON", 0, 0, __64con_putc, \
	__64con_getc, __64con_init, __64con_exit };

#define 64DISK { \
	"64DISK", 0, 0, __64disk_writeb, \
	__64disk_readb, __64disk_init, __64disk_exit };
