/*
 * mount.c
 * mounting/unmounting filesystems
 * 12/03/03
 */

#include <kernel.h>

void k_mount_init () {

	// mount root here

	struct s_mount mount;

	if (!(mount.sb = k_readsb (NULL, c_ROOTFSTYPE)))
		k_panic ();

	mount.target = k_cachedir (mount.sb.rootdir);

	mount.source = k_path_to_dentry (c_ROOTDEVFILE);

	mount.fstype = c_ROOTFSTYPE;

	if (k_kernel.nmntlistentry == c_MAXNMNTLISTENTRY)
		k_panic ();

	k_kernel.mountlist [0] = mount;
	k_kernel.nmntlistentry++;
}

int k_mount_mount (struct s_dentry * mountpoint, struct s_dentry * source, int fstype) {

	/* missing:
	 * check for existance of target- and sourcepoints
	 */
	
	struct s_mount mount;

	if (k_kernel.nmntlistentry == c_MAXNMNTLISTENTRY) {

		k_setlasterror (c_ERRMAXNMNTLISTENTRYREACHED);
		return 0;
	}
	
	switch fstype {

#ifdef c_CFGFS1541FS
		case c_FS1541FS:
#endif
#ifdef c_CFGFSPFS
		case c_FSPFS:
#endif	
		mount.fstype = fstype;

		return -1;

		default:
			k_setlasterror (c_ERRUNKNOWNFS);
			return 0;
	}

	mount.source = source;
	mount.mountpoint = mountpoint;

	mount.superblock = k_readsuper (mount.source);
	k_cachedir (mountpoint);

	k_kernel.mountlist [k_kernel.nmntlistentry] = mount;
	k_kernel.nmntlistentry++;
}

int k_mount_unmount (struct s_dentry * mountpoint) {

	// WARNING: may only unmount if opened
	// files on mount == 0
	
	unsigned long i;

	for (i = 0; i < c_MAXNMNTLISTENTRY; i++)
		if (k_kernel.mountlist [i].target == dentry) {
	
			k_kernel.mountlist [i] = k_kernel.mountlist [k_kernel.nmountlistentry - 1];
			k_kernel.nmountlistentry--;
			k_uncachedir (mountpoint);
			return TRUE;
		}

	return FALSE;
}

// this is probably going to be dropped, see ./todo.txt

struct s_mount * k_mount_getmount (struct s_dentry * dentry) {

	struct s_dentry * cur_dentry;
	unsigned char i;

	cur_dentry = dentry;
		
	while (cur_dentry->parent != NULL) {
		
		for (i = 0; i < c_MAXNMNTLISTENTRY; i++)
			if (cur_dentry == k_kernel.mountlist [i].target)
				return &k_kernel.mountlist [i];

		cur_dentry = cur_dentry->parent;
	}
}
