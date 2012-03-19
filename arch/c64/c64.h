/*
* c64.h
* 01/14/03
* c64 machine include file
*/

/* the c64 needs relocation */

#define OPT_RELOCATE

/* maximum task stack size */

#define c_MAX_STACKSIZE 128

/* kind of memory adress bus: */

#define	__16BITMEM

/* this defines where the kernel should start
	when searching free memory */

#define c_MEMFREESTART 0x0020 // of course, adjust this to kernel memory

/* kernel base adress: */

#define c_KRNLBASE 0xc000

/* nmi vector: */

/*#define c_NMIVECTOR 0xfffa*/

/* irq vector: */

/*#define c_IRQVECTOR 0xfffe*/

/* reset vector: */

/*#define c_RESETVECTOR 0xfffc*/
