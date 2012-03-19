/*
* pOS
* sysinit.c
* 03/23/02
*/

#include <kernel.h>

// main entry point

s_kernel k_kernel;

void k_start_kernel () {

	s_file * init;
	void * buffer;

	k_lock;

	k_kernel.kernelmode = TRUE;

	k_kernel.nopenfiles = 0; // there should be a instance
	k_kernel.nprocs = 0; // of s_kernel with standard values
				// (like for taskenv, signals etc)

	k_meminit (); // prepare memory (read /kernel/todo.txt why !)
	k_drvinit (NULL); // initialize drivers
	k_mountinit ();
	k_consetdev (c_DEFCONDEV);
	k_showflags (); // display compiled flags

	k_conput ("pOS - VERSION: " c_STRKERNELVERSION);
	k_conput (c_STRKERNELCOPYRIGHT);

	k_conput ("LOADING INIT...");

	/* This should all reside in exec ():

        init = k_openfile ("/init", 0);
	buffer = k_malloc (k_getfilesize (init));
        k_readfile (init, buffer, k_getfilesize (init));
	k_addtask (buffer, c_PRIORITY_NORMAL);
        // k_addtask is absolutely wrong here
        // if, then it should read k_createproc () !!!

	*/

        exec ("/init", c_PRIORITY_NORMAL);
}
