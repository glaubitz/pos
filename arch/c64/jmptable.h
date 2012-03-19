/*
* pOS
* jmptable.h - c64 jump table header
* 01/14/03
*/

// THIS FILE MUST NOT BE INCLUDED BY THE KERNEL

// DO NOT EDIT

#include "c64.h"

#define j_reboot c_KRNLBASE + 0
#define j_addtask c_KRNLBASE + 3
#define j_createfile c_KRNLBASE + 6
#define j_createproc c_KRNLBASE + 9
#define j_deletefile c_KRNLBASE + 12
#define j_free c_KRNLBASE + 15
#define j_freedevice c_KRNLBASE + 18
#define j_getc c_KRNLBASE + 21
#define j_getdevice c_KRNLBASE + 24
#define j_getfilelength c_KRNLBASE + 27
#define j_getfreemem c_KRNLBASE + 30
#define j_getlasterror c_KRNLBASE + 33
#define j_getproc c_KRNLBASE + 36
#define j_getreservedmem c_KRNLBASE + 39
#define j_gettime c_KRNLBASE + 42
#define j_getusedmem c_KRNLBASE + 45
#define j_killproc c_KRNLBASE + 48
#define j_initdisk c_KRNLBASE + 51
#define j_isowner c_KRNLBASE + 54
#define j_malloc c_KRNLBASE + 57
#define j_memset c_KRNLBASE + 60
#define j_putc c_KRNLBASE + 63
#define j_readblock c_KRNLBASE + 66
#define j_readfile c_KRNLBASE + 69
#define j_remalloc c_KRNLBASE + 72
#define j_removetask c_KRNLBASE + 75
#define j_renamefile c_KRNLBASE + 78
#define j_sendsignal c_KRNLBASE + 81
#define j_setlasterror c_KRNLBASE + 84
#define j_settime c_KRNLBASE + 87
#define j_writeblock c_KRNLBASE + 90
#define j_writefile c_KRNLBASE + 93
