/*
* pOS
* pos.c - pOS static libary
* 01/22/03
*/

// DO NOT EDIT
                                                               
#include <pos.h>
#include "../ARCH/$MACHINE/jmptable.h"

void (* reboot) (void) = (void (*) (void)) j_reboot;
TASKINFO * (* addtask) (void * heap, PROCINFO * owner) = (TASKINFO * (*) (void *, PROCINFO *)) j_addtask;
FILEINFO * (* createfile) (BYTE drive, char * filename, WORD attributes) = (FILEINFO * (*) (BYTE, char *, WORD)) j_createfile;
PROCINFO * (* createproc) (char * filename, BYTE priority) = (PROCINFO * (*) (char *, BYTE)) j_createproc;
int (* deletefile) (FILEINFO * file) = (int (*) (FILEINFO *)) j_deletefile;
int (* free) (void * object) = (int (*) (void *)) j_free;
int (* freedevice) (DEVINFO * device) = (int (*) (DEVINFO *)) j_freedevice;
char (* getc) (DEVINFO * device) = (char (*) (DEVINFO *)) j_getc;
DEVINFO * (* getdevice) (char * name) = (DEVINFO * (*) (char *)) j_getdevice;
DWORD (* getfilelength) (FILEINFO * file) = (DWORD (*) (FILEINFO *)) j_getfilelength;
MEMVAR (* getfreemem) (void) = (MEMVAR (*) (void)) j_getfreemem;
BYTE (* getlasterror) (void) = (BYTE (*) (void)) j_getlasterror;
PROCINFO * (* getproc) (BYTE pid) = (PROCINFO * (*) (BYTE)) j_getproc;
MEMVAR (* getreservedmem) (MEMBINFO * pmembinfo) = (MEMVAR (*) (MEMBINFO *)) j_getreservedmem;
TIMEINFO * (* gettime) (TIMEINFO * time) = (TIMEINFO * (*) (TIMEINFO *)) j_gettime;
MEMVAR (* getusedmem) (MEMBINFO * pmembinfo) = (MEMVAR (*) (MEMBINFO *)) j_getusedmem;
int (* killproc) (PROCINFO * proc, WORD flags) = (int (*) (PROCINFO *, WORD)) j_killproc;
int (* initdisk) (BYTE drive) = (int (*) (BYTE)) j_initdisk;
int (* isowner) (PROCINFO * child, PROCINFO * parent) = (j_isowner;
void * (* malloc) (MEMVAR size) = (void * (*) (MEMVAR)) j_malloc;
int (* memset) (void * object, MEMVAR size, BYTE value) = (int (*) (void *, MEMVAR, BYTE)) j_memset;
int (* putc) (DEVINFO * device, char c) = (int (*) (DEVINFO *, char)) j_putc;
WORD (* readblock) (DEVINFO * device, BYTE * buffer, WORD nbytes) = (WORD (*) (FILEINFO *, BYTE *, WORD)) j_readblock;
DWORD (* readfile) (FILEINFO * file, BYTE buffer []) = (DWORD (*) (FILEINFO *, BYTE [])) j_readfile;
void * (* remalloc) (void * object, MEMVAR size) = (void * (*) (void *, MEMVAR)) j_remalloc;
void (* removetask) (TASKINFO * task) = (void (*) (TASKINFO *)) j_removetask;
int (* renamefile) (FILEINFO * file, char * newname) = (int (*) (FILEINFO *, char *)) j_renamefile;
int (* send_signal) (TASKINFO * process, BYTE signal) = (int (*) (TASKINFO *, BYTE)) j_sendsignal;
void (* setlasterror) (BYTE error) = (void (*) (BYTE)) j_setlasterror;
void (* settime) (TIMEINFO * newtime) = (void (*) (TIMEINFO *)) j_settime;
int (* writeblock) (DEVINFO * device, BYTE * buffer, WORD nbytes) = (int (*) (DEVINFO *, BYTE *, WORD)) j_writeblock;
DWORD (* writefile) (FILEINFO * file, BYTE buffer []) = (DWORD (*) (FILEINFO *, BYTE [])) j_writefile;
