/*
 * kernel.h
 * 01/15/05
 * kernel include file
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#include <config.h>
#include <const.h>
#include <types.h>

#define c_initsignals \
{ \
\
                k_sig_sleep, k_sig_wakeup, k_sig_kill, \
                k_sig_user, k_sig_user \
}

/* pOS functions */

/* console */

int k_console_redirect (char * filename);
int k_console_put (char * s);
int k_console_get (char * s, unsigned char bufsize);

/* dcache */

int k_dcache_init ();
int k_dcache_mkdentry (struct s_dentry * parent, struct s_dentry * dentry);
int k_dcache_rmdentry (struct s_dentry * dentry);
int k_dcache_chkupdate (s_dentry * dentry);
unsigned char k_dcache_getattr (struct s_dentry * dentry);
unsigned long k_dcache_getsize (struct s_dentry * dentry);
char * k_dcache_getfilename (struct s_dentry * dentry);
int k_dcache_findnext (char * pattern, struct s_dentry * dentry);
int k_dcache_getdir (struct s_dentry * dentry, struct s_dir * dir);
int k_dcache_cachedir (struct s_dentry * dir);
int k_dcache_uncachedir (struct s_dentry * dir);
struct s_dentry * k_path_to_dentry (char * path);
char * k_dentry_to_path (struct s_dentry * dentry, char * path, unsigned long bufsize);

/* devices */

int k_device_init (struct s_device * device);
int k_device_exit (struct s_device * device);
struct s_device k_device_get (unsigned char major, unsigned char minor);
unsigned long k_device_read (struct s_device * device, unsigned char * buffer,
				unsigned long buffersize, unsigned long offset);
unsigned long k_device_write (struct s_device * device, unsigned char * buffer,
				unsigned long buffersize, unsigned long offset);

/* file */

int k_file_open (char * filename, struct s_file * file, int mode);
int k_file_close (struct s_file * file);
int k_file_rmfile (char * filename);
int k_file_mkdir (char * dirname);
int k_file_rmdir (struct s_dentry * dir);
int k_file_rename (char * filename, char * newname);
int k_file_readfile (struct s_file * file, void * buffer, unsigned long bufsize);
int k_file_writefile (struct s_file * file, void * buffer, unsigned long bufsize);

/* filesystem */

int k_filesystem_readsb (struct s_dentry * filesystem, int filesystemtype);
int k_filesystem_readdir (struct s_dentry * dir, void * buffer);
int k_filesystem_updatedentry (struct s_dentry * dentry);
int k_filesystem_mkdir (struct s_dentry * parent, struct s_dentry * newdir,
			char * name);
int k_filesystem_rmdir (struct s_dentry * dir);
int k_filesystem_mkfile (struct s_dentry * parent, struct s_dentry * newfile,
			char * name);
int k_filesystem_rmfile (struct s_dentry * file);
int k_filesystem_rename (struct s_dentry * dentry, char * newname);
int k_filesystem_readfile (struct s_dentry file, void* buffer, unsigned long bufsize,
			unsigned long offset);
int k_filesystem_writefile (struct s_dentry file, void* buffer, unsigned long bufsize,
			unsigned long offset);

/* memory */

void k_memory_init ();
void * k_memory_alloc (unsigned long size, unsigned long reserve);
void k_memory_set (void * object, unsigned long size, unsigned char value);
void k_memory_copy (void * source, void * target, unsigned long size);
int k_memory_free (void * object);
void * k_memory_realloc (void * object, unsigned long size);
unsigned long k_memory_getfree ();
unsigned long k_memory_getreserved (struct s_mem * pmembinfo);
unsigned long k_memory_getused (struct s_mem * pmembinfo);


/* mount */

void k_mount_init ();
int k_mount_mount (struct s_dentry * mountpoint, struct s_dentry * source, int fstype);
int k_mount_unmount (struct s_dentry * mountpoint);
struct s_mount * k_mount_getmount (struct s_dentry * dentry);

/* panic */

void k_panic_panic ();

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

struct s_time * k_time_get (struct s_time * time);
int k_time_set (struct s_time * newtime);


struct s_kernel {

	/* when in this mode all devices belong to the kernel
		and task-switching is disabled 
		see also /pos/kernel/kmode.c	
	*/

	int kernelmode;

	unsigned char lasterror;

	unsigned char nopenfiles;
	unsigned char nprocs;
	unsigned char ntasks;
	unsigned char ndrivers;
	unsigned char nmntlistentry;

        /* the following are buffers to find faster
                id's for new processes/tasks */

	unsigned char lastfreepid;
	unsigned char lastfreetid;

	struct s_driver driverlist [c_MAXNDEVICE];
	struct s_mount mountlist [c_MAXNMNTLISTENTRY];
	struct s_file filelist [c_MAXNOPENFILES];

	struct s_proc procs [c_MAXNPROCS];
	struct s_task tasks [c_MAXNTASKS];

	unsigned char icurrent;
	struct s_task * current;
        unsigned char current_ttl;

	struct s_file * console;

	struct s_time boottime;
	unsigned long tickcount;

	void * pdcache; // pointer to directory-cache (must not be changed !)
	struct s_dentry * rootdir;
};

#endif
