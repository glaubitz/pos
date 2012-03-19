/*
* pOS
* dcache.c - directory cache
* 05/04/05
*/

// Missing: Error handling !
// Missing2: Syncing to disk after changes to directory-cache

#include <kernel.h>
#include <string.h>

int k_dcache_init () {

	// allocate memory for dcache
	// PANIC here on failure !

	if (!k_kernel.pdcache = k_malloc (c_MAXDIRCACHED * sizeof (s_dir)))
		k_panic_panic ("Failed to allocate memory for directory cache");
}

int k_dcache_mkdentry (struct s_dentry * parent, struct s_dentry * dentry) {

	struct s_dir * parent_dir;

	k_dcache_getdir (parent, parent_dir);
	
	if (parent_dir->entrycount < c_MAXDIRENTRY) {
	
		dentry = &parent->entries [parent->entrycount];
		parent->entrycount++;
		return -ENOERROR;
	}
	else
		// a typical case where we need error return codes
		return -EDIRFULL;
}

int k_dcache_rmdentry (struct s_dentry * dentry) {

	*dentry = parent->entries [parent->entrycount - 1];
	parent->entrycount--;
}

int k_dcache_chkupdate (s_dentry * dentry) {

	// WARNING: may only be called by
	// opening task/process
	//
	// 10/11/05: probably obsolete
	
	int retval = dentry->update;

	dentry->update = FALSE;

	return retval;
}

unsigned char k_dcache_getattr (struct s_dentry * dentry) {

	return dentry->attr;
}

unsigned long k_dcache_getsize (struct s_dentry * dentry) {

	return dentry->size;
}

int k_dcache_getfilename (struct s_dentry * dentry, char * filename) {

	return strcpy (&dentry->filename, filename);
}

int k_dcache_findnext (char * pattern, struct s_dentry * dentry) {

	// LOW memory filesystem search, *untested*
	
	// Limitations:
	// - ignores the name of the current node !
	// - can only search for single dentry-names (no pathnames)

	struct s_dir * start_dir, cur_dir;
	int dentry_count;

	k_dcache_getdir (dentry->parent, start_dir);
	cur_dir = start_dir;
	
	dentry_count = (dentry - &start_dir->entries [0]) / sizeof (s_dentry);
	
	do {
		if (k_isdir (dentry)) {
			k_dcache_getdir (dentry, cur_dir);
			dentry = cur_dir->entries [0];
			dentry_count = 0;
		}
		else {
			if (dentry_count < cur_dir->entrycount)
				dentry = cur_dir->entries [dentry_count];
			else {
				if (cur_dir == start_dir)
					return 0;
				else {
					/* the loop is necessary in case the parent directory
					   is also the last dentry of it's parent thus we
					   have to climb-up even more until we reach a level
					   where we're not the last dentry
					 */
					
					do {
						dentry = dentry->parent; // gives us next dentry of parent directory
						k_dcache_getdir (dentry->parent, cur_dir); // go up one directory
						dentry_count = 1 + (dentry - &cur_dir->entries [0]) / sizeof (s_dentry);
						// go to successor of parent dentry, if the parent is already
						// the last of it's parent, the loop will repeat else break
					} while (dentry_count = cur_dir->entrycount);
				}
			}
		}

		dentry_count++;

	} while (strcmp (dentry->name, pattern) != 0);
	
	return 0;
}

int k_dcache_getdir (struct s_dentry * dentry, struct s_dir * dir) {

	if (k_dcache_isdir (dentry)) {
		if (!dentry->in_cache)
			k_dcache_cachedir (dentry);
			
		dir = dentry->cached_dir;

		return -ENOERROR;
	}

	return -ENOSUCHDIR;
}

int k_dcache_cachedir (struct s_dentry * dir) {

	// read sub-directory into dcache
	
	struct s_dir * pdir;
	unsigned long i;

	if (!k_isdir (dir))
		return FALSE;

	pdir = (struct s_dir *) k_kernel.pdcache;

	for (i = 0; i < c_MAXDIRCACHED; i++)

		if (pdir [i].cache_expired) {
		
			dir->cached_dir = &pdir [i];
			k_fs_readdir (dir, pdir); // what if fails here ?
			return -ENOERROR;
		};

	return -ECANNOTCACHE;
}

int k_dcache_uncachedir (struct s_dentry * dir) {

	// set expired-flag of directory AND sub-dirs recursively:
	// this routine is slow but requires LOW memory (and stack!)

	struct s_dentry * cur_dir;
	unsigned long i;

	cur_dir = dir;

	do {
		i = 0;
		while (i < cur_dir->dentrycount) {
			if (!cur_dir->entries [i].cache_expired &&
					k_isdir (cur_dir->entries [i])) {

				cur_dir->cache_expired = TRUE;
				cur_dir = &cur_dir->entries [i];
				i = 0;
				continue;
			}

			// could do the uncaching of files in filebuffer here

			i++;
		}

		cur_dir = cur_dir->parent;

	} while (cur_dir != dir->parent);
}

struct s_dentry * k_path_to_dentry (char * path) {

	struct s_dentry * cur_dentry;
	struct s_dir * cur_dir;
	unsigned long i, j;

	cur_dentry = k_getrootdir ();
	i = 0;

	if (path [0] != '/')
		return NULL;
	
	while (1) {

		if (path [i] == '/')
			i++;

		if (path [i] == '\0')
			return cur_dentry;

		if (!is_dir (cur_dentry))
			return NULL;

		cur_dir = cur_dentry->cached_dir;

		for (j = 0; j < cur_dir->entrycount; j++) {
	
			if (strncmp (cur_dir->entries [j].name,
				path + i, strlen (cur_dentry->name)) == 0) {

				cur_dentry = &cur_dir.entries [j];
				i += strlen (cur_dentry->name);
				break;
			}
		}

		if (j == cur_dir->entrycount)
			return NULL;
	}
}

char * k_dentry_to_path (struct s_dentry * dentry, char * path, unsigned long bufsize) {

	struct s_dentry * cur_dentry;
	unsigned long i;

	cur_dentry = dentry;
	i = 0;
	path = "/\0";

	while (cur_dentry->parent != NULL) {

		i += strlen (cur_dentry->name) + 1;
		cur_dentry = cur_dentry->parent;
	}

	if (i > bufsize)
		return NULL;

	while (cur_dentry->parent != NULL) {

		i -= strlen (cur_dentry->name);
		strcpy (cur_dentry->name, path [i]);
		i--;
		path [i] = '/';
	}

	return path;
}
