/*
 * 1541fs.h
 * VC1541 Filesystem Driver
 * 12/03/03
 */

struct s_1541fs_sb {

	char diskname [18];
	char id [2];
};

struct s_1541fs_dentry {

	char filename [16];
};
