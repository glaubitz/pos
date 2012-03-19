/*
 * pOS
 * pos.c - pOS static libary
 * 02/12/03
 */

// DO NOT EDIT
                                                               
#include <pos.h>
#include "../ARCH/$MACHINE/jmptable.h"

void (* reboot) (void) = j_reboot;
struct s_task * (* addtask) (void * heap, struct s_proc * owner) = j_addtask;
struct s_file * (* createfile) (unsigned char drive, char * filename, int attributes) = j_createfile;
struct s_proc * (* createproc) (char * filename, unsigned char priority) = j_createproc;
int (* deletefile) (struct s_file * file) = j_deletefile;
int (* free) (void * object) = j_free;
int (* freedevice) (struct s_device * device) = j_freedevice;
char (* getc) (struct s_device * device) = j_getc;
struct s_device * (* getdevice) (char * name) = j_getdevice;
unsigned long (* getfilelength) (struct s_file * file) = j_getfilelength;
MEMVAR (* getfreemem) (void) = j_getfreemem;
unsigned char (* getlasterror) (void) = j_getlasterror;
struct s_proc * (* getproc) (unsigned char pid) = j_getproc;
MEMVAR (* getreservedmem) (MEMBINFO * pmembinfo) = j_getreservedmem;
struct s_time * (* gettime) (struct s_time * time) = j_gettime;
MEMVAR (* getusedmem) (MEMBINFO * pmembinfo) = j_getusedmem;
int (* killproc) (struct s_proc * proc, int flags) = j_killproc;
int (* initdisk) (unsigned char drive) = j_initdisk;
int (* isowner) (struct s_proc * child, struct s_proc * parent) = j_isowner;
void * (* malloc) (MEMVAR size) = j_malloc;
int (* memset) (void * object, MEMVAR size, unsigned char value) = j_memset;
int (* putc) (struct s_device * device, char c) = j_putc;
int (* readblock) (struct s_device * device, unsigned char * buffer, int nunsigned chars) = j_readblock;
unsigned long (* readfile) (struct s_file * file, unsigned char buffer []) = j_readfile;
void * (* remalloc) (void * object, MEMVAR size) = j_remalloc;
void (* removetask) (struct s_task * task) = j_removetask;
int (* renamefile) (struct s_file * file, char * newname) = j_renamefile;
int (* send_signal) (struct s_task * process, unsigned char signal) = j_sendsignal;
void (* setlasterror) (unsigned char error) = j_setlasterror;
void (* settime) (struct s_time * newtime) = j_settime;
unsigned long (* writeblock) (struct s_device * device, unsigned char * buffer, unsigned long nunsigned chars) = j_writeblock;
unsigned long (* writefile) (struct s_file * file, unsigned long buffer []) = j_writefile;
