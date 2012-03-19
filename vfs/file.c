/*
* pOS
* file.c - file operations
* 11/27/03
*/

/*
        TODO:
        - adjust struct s_file in include/types.h
        - write k_find, k_rename, k_delete
        - allow relative pathnames ? (or is this only done through the shell ?)
	- add support for the mode-parameter in k_open
	- create a standard fill-in template for the s_file-struct
*/

#include <kernel.h>

int k_file_open (char * filename, struct s_file * file, int mode) {

	struct s_dentry * dentry;

	// ERROR: must remove name from path here as name does not
	// exist on creating !!
	
	dentry = k_path_to_dentry (filename);

	switch (mode ^ (!%11)) {

		case c_FFLAGOPEN:
			if (!dentry)
				return FALSE;
			break;

		case c_FFLAGCREATE:
			if (dentry)
				return FALSE;
			break;

		case c_FFLAGOVERWRITE:
			break;

		default:
			return FALSE;
	}

	if (dentry->attr | c_ATTRDIR)
		return FALSE; // we cannot open directories for i/o

	if (!(dentry = k_path_to_dentry (filename)) && (mode | c_FFLAGOPEN))
			return FALSE;
	else {

		file->dentry = dentry;
		file->flags = mode;

		// missing: check whether selected open-
		// mode
		return TRUE;
	}
}

int k_file_close (struct s_file * file) {

	// at the moment i know nothing
	// to do here as we don't have
	// a file-cache flush to disk
	// or similar
	// ehem, we've got to update
	// the dentry in dcache !!!
}

int k_file_rmfile (char * filename) {

	struct s_dentry * file;

	if (!(file = k_path_to_dentry (filename)))
			return FALSE;
	else {

		// 1. call fs-api to delete driver

		k_dcache_rmdentry (file);
	}
}

int k_file_mkdir (char * dirname) {

	// 1. convert into path
	// 2. mkdir on fs-api
	
	k_dcache_mkdentry (dir);
}

int k_file_rmdir (struct s_dentry * dir) {

	// 1. rmdir on fs-api
	// 2. rmdir on dcache

	k_dcache_rmdentry (dir);
}

int k_file_rename (char * filename, char * newname) {

	struct s_dentry * dentry;

	// WARNING: must return error if file is open !

	if (!(dentry = k_path_to_dentry (filename)))
			return FALSE;
	else {
		
		// rename on fs-api
		k_dcache_rndentry (dentry, newname);
	}
}

int k_file_readfile (struct s_file * file, void * buffer, unsigned long bufsize) {

	// anything to add here ?
	
	return k_fs_readfile (file->dentry, buffer, bufsize);
}

int k_file_writefile (struct s_file * file, void * buffer, unsigned long bufsize) {

	return k_fs_writefile (file->dentry, buffer, bufsize);
}
