/*
 * types.h
 * 12/03/03
 * pOS standard types
 */

#ifndef __TYPES_H
#define __TYPES_H

#define NULL ((void *) 0)
#define TRUE -1
#define FALSE 0

#ifdef c_FS1541FS
#include <fs/1541fs.h>
#endif

struct s_device;
struct s_dir;
struct s_driver;
struct s_file;
struct s_mem;
struct s_mount;
struct s_proc;
struct s_sb;
struct s_task;
struct s_time;

struct s_device {

	struct s_driver * driver;
	unsigned char minor;
};

struct s_dentry {

	char name [c_MAXFILENAMELENGTH];
	struct s_dentry * parent;
	unsigned char attr; // d-rwx-rw e.g
	unsigned long size; // filesize (= 0 for dirs)
	int in_cache; // if this dentry is a dir, is it in dcache ?
			// may also be used for a file-cache later
	int update;

	union {

		struct s_dir * cached_dir;
		// struct s_cachefile * cached_file;
	};

	union  {
		
		struct s_1541fs_dentry 1541fs_dentry; // dentry of 1541-fs-driver
		// struct s_myfs_dentry myfs_dentry // dentry of custom fs-driver
	};	
};

struct s_dir {

//	struct s_dentry * parent; // a parent dir is always in cache !, =NULL if root in VFS
	struct s_dentry entries [c_MAXDIRENTRY]; // this directory's entries
	unsigned long entrycount;
	int cache_expired; // if expired, this directory can be overwritten in cache
};

struct s_driver {

	unsigned char highestminor;
	int type;

	union {
		unsigned long (* chardev) (unsigned char minor,
				unsigned char * buffer, unsigned long bufsize);
		unsigned long (* blockdev) (unsigned char minor,
				unsigned char * buffer, unsigned long bufsize, unsigned long offset);
	} read;

	union {
		unsigned long (* chardev) (unsigned char minor,
				unsigned char * buffer, unsigned long bufsize);
		unsigned long (* blockdev) (unsigned char minor,
				unsigned char * buffer, unsigned long bufsize, unsigned long offset);
	} write;

	int (* init) ();
	int (* exit) ();

};

struct s_file {

       // char filename [c_MAXPATHLENGTH];
	unsigned long size;
        unsigned long offset;
	int flags;

	struct s_dentry * dentry;
/*
        struct s_mount * mountinfo; // still need this ?

	union {
		
		struct s_device device;
#ifdef c_FSPFS
		struct s_fspfs pfs;
#endif
	} fs;
*/
};

struct s_mem {

	unsigned char pid;
	unsigned long size;

};

struct s_mount {

	struct s_dentry * source;
	struct s_dentry * mountpoint;
	int flags;
	struct s_sb sb;
	int loopback; // is loopback
	union {

		struct s_file sourcefile;
		struct s_device sourcedevice;
	}
};

struct s_proc {

	unsigned char pid;
	unsigned char ppid;
	unsigned char priority; // 1...9 = factor for currentttl
	unsigned char lasterror;
	void (void) signals [5];
};

struct s_sb {

	int fstype;
	// unsigned long dentrycount; // do we need this ???, i dont think so
	struct s_dentry rootdir; // dentry of root-directory of filesystem
				// (not to mix up with VFS-root of kernel-object !)
	union {

		struct s_1541fs_sb 1541fs_sb; // superblock of 1541-fs-driver
		// struct s_myfs_sb myfs_sb; // superblock of custom fs-driver
	};
};

struct s_task {

	struct s_taskenv taskenv;
	void * heap;
	unsigned char tid;
	struct s_proc * owner; // to access data in the owner's proc struct
};

struct s_time {

	unsigned char years;
	unsigned char months;
	unsigned char days;
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
};

#endif
