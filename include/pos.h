/*
 * pos.h
 * 01/15/05
 * application include file
 */

#include <config.h>
#include <const.h>
#include <types.h>

// This is rather outdated, don't rely on, better check out kernel.h
// pos.h will be updated as soon as all major code has been completed

/* TODO:
 *
 * - keep up-to-date with kernel.h
 * - remove non-userspace callable functions
 */

/* pOS functions */

/* console */

int consetdev (char * confile);
int conput (char * s);
int conget (char * s, unsigned char bufsize);

/* devices */

int drvinit (struct s_device * device);
int drvexit (struct s_device * device);
struct s_device getdevice (unsigned char major, unsigned char minor);
unsigned long readdevice (struct s_device * device, unsigned char * buffer,
				unsigned long buffersize, unsigned long offset);
unsigned long writedevice (struct s_device * device, unsigned char * buffer,
				unsigned long buffersize, unsigned long offset);

/* memory */

void meminit ();
void * malloc (unsigned long size, unsigned long reserve);
void memset (void * object, unsigned long size, unsigned char value);
void memcpy (void * source, void * target, unsigned long size);
int free (void * object);
void * remalloc (void * object, unsigned long size);
unsigned long getfreemem ();
unsigned long getreservedmem (struct s_mem * pmembinfo);
unsigned long getusedmem (struct s_mem * pmembinfo);

/* mount */

void mountinit ();
int mount (char * target, char * source, int fstype);
int unmount (char * target);

/* process */

struct s_proc * k_createproc (char * pathname, unsigned char priority);
int k_killproc (struct s_proc * proc, int flags);
int k_isowner (struct s_proc * child, struct s_proc * parent);
struct s_proc * k_getproc (unsigned char pid);
unsigned char k_getlasterror ();
void k_setlasterror (unsigned char error);

/* signals */

int k_sendsignal (struct s_task * process, unsigned char signal);

/* tasks */

struct s_task * k_addtask (void * heap, struct s_proc * owner);
void k_removetask (struct s_task * task);

/* time */

struct s_time * k_gettime (struct s_time * time);
void k_settime (struct s_time * newtime);

/* vfs */

struct s_file * open (char * pathname, int flags);
int close (struct s_file * file);
unsigned long read (struct s_file * file, void * buffer, unsigned long nbytes);
unsigned long write (struct s_file * file, void * buffer, unsigned long nbytes);
