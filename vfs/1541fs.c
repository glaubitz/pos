/*
* pOS
* 1541fs.c - VC1541 filesystem driver
* 12/04/03
*/

/*
 NOTE:

 this driver accesses VC1541 directly by programming
 CIA and sending conventional 1541 commands for LOAD/SAVE/
 SCRATCH/RENAME etc and circumvents the block-driver
 1541block.s, see bugs.txt for more on this

 use file:/root/proj/other/lunix/kernel/fs_iec.s as reference !

*/

#include <kernel.h>

static int 1541_recv (void * buffer, unsigned long bufsize) {


}

static int 1541_send (void * buffer, unsigned long bufsize) {



}

int 1541fs_readsb (struct s_dentry * dentry) {

}

int 1541fs_updatedentry (struct s_dentry * dentry) {

	// reread dentry from disk
}

int 1541fs_mkdir (struct s_dentry * parent) {

	// 1541fs has no directories

	return 0;
}

int 1541fs_rmdir (struct s_dentry * dir) {

	return 0;
}

int 1541fs_mkfile (char * filename, struct s_dentry * parent) {

	return 0;
}

int 1541fs_rmfile (struct s_dentry * file) {

	char command [256];

	strcpy (command, "S:");
	strcpy (command, file->1541fs_dentry.filename);

	return 1541_send (command);
}

int 1541fs_rename (struct s_dentry * dentry, char * newname) {

	char command [256];

	strcpy (command, "R:");
	strcpy (command, file->1541fs_dentry.filename);
	strcpy (command, "=");
	strcpy (command, newname);

	return 1541_send (command);
}

int 1541fs_readdir (struct s_dentry * dentry, void * buffer) {

	// read root-dir of device into mem

	return 0;
}

int 1541fs_readfile (struct s_dentry * file, void * buffer,
			unsigned long bufsize, unsigned long offset) {

	// open file implicitly
}

int 1541fs_writefile (struct s_dentry * file, void * buffer,
			unsigned long bufsize, unsigned long offset) {

	// open file implicitly
}
