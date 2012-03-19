/*
* pOS
* filesystem.c - filesystem backend
* 12/19/04
*/

#include <kernel.h>

// NOTE: for determination of the proper filesystem
// routines is done using simple switch-constructs
// (i don't want to introduce another struct
// for that !), maybe we can do that with some
// macros ?!?

int k_filesystem_readsb (struct s_dentry * filesystem, int filesystemtype) {

	// read-superblock of filesystem,
	// if filesystem == NULL, read super
	// from root-device as defined
	// in include/config.h
	// (problem yet is that in config.h
	// the root-device is defined
	// in major/minor format, whereas
	// the filesystem-routines expect s_dentry)
	//
	// NOT REALLY: we'll convert from
	// dentry into file/dev here
	// with k_mount_getmount and pass
	// p to dev/file-struct and
	// loopback-flag (int-boolean)
	// filesystem-routines
	//
	// UPDATE: the latter is wrong also,
	// we don't use any of these functions
	// when mounting root, we just read
	// sb directly with direct block-dev i/o
	// with the functions from kernel/dev.c
	// which accept s_dev-structs
	
	if (!filesystem) {
		block_dev = c_ROOTDEVSTRUCT;

		// read sb from a block-device
		// 1. call read_sb from respective filesystem-driver
		
		switch (filesystem) {
			default:;
		}
	}
	else {
		// read sb from a file (maybe bd or image !)
	}
}

int k_filesystem_readdir (struct s_dentry * dir, void * buffer) {

	// read directory into buffer (called by cachedir)
}

int k_filesystem_updatedentry (struct s_dentry * dentry) {

	// updates dentry from disc
	// called from
	// k_file_close, k_file_rename
}

int k_filesystem_mkdir (struct s_dentry * parent, struct s_dentry * newdir,
			char * name) {

	// 1. determine filesystem-driver
	// 2. perform mkdir
}

int k_filesystem_rmdir (struct s_dentry * dir) {

	// 1. determine filesystem-driver
	// 2. perform rmdir
}

int k_filesystem_mkfile (struct s_dentry * parent, struct s_dentry * newfile,
			char * name) {

	// 1. determine filesystem-driver
	// 2. perform mkfile
}

int k_filesystem_rmfile (struct s_dentry * file) {

	// 1. determine filesystem-driver
	// 2. perform rmfile
}

int k_filesystem_rename (struct s_dentry * dentry, char * newname) {

	// 1. determine filesystem-driver
	// 2. perform rename
}

int k_filesystem_readfile (struct s_dentry file, void* buffer, unsigned long bufsize,
			unsigned long offset) {

	// 1. determine filesystem-driver
	// 2. perform readfile
}

int k_filesystem_writefile (struct s_dentry file, void* buffer, unsigned long bufsize,
			unsigned long offset) {

	// 1. determine filesystem-driver
	// 2. perform writefile
}
