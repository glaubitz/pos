/*
 * pOS
 * process.c
 * 07/24/03
 */

// This is garbage and need's a total redesign !!! (No real update since about 2001)

/*
todo:
- finish k_createproc and k_killproc
*/

#include <kernel.h>

struct s_proc * k_createproc (char * pathname, unsigned char priority) {

	struct s_proc * process;
	TASKINFO * task;
	FILEINFO * procfile;
	void * heap;
	unsigned char pid;

	MEMVAR i, heapsize;
        
        if (k_kernel.nprocs == c_MAX_NPROCS) {
		
                k_setlasterror (c_ERRMAXNPROCSREACHED);
		return NULL;
	}

	if (priority > 9 || !priority) {

                k_setlasterror (c_ERRILLEGALPARAMETER);
		return NULL;
	}

	// what happens when several processes
	// have been killed subsequently and
	// we increment highestpid, we will
	// reach 255 and then 0, this must
	// not happen, so if lastfreepid == 0
	// then we have to search before we
	// increment

        if (k_kernel.lastfreepid == 0) {
                for (pid = 0; pid < c_MAXNPROCS; pid++) {

                        for (i = 0; i < k_kernel.nprocs; i++)
                                if (k_kernel.procs [i].pid == pid)
					break;

                        if (i == k_kernel.nprocs) { // no match found -> pid is free

				process->pid = pid;
				break;
			}
		}
	}
	else {
			
                process->pid = k_kernel.lastfreepid;
                k_kernel.lastfreepid = 0;
	}

        process->ppid = k_kernel.current->owner->pid;

        procfile = k_open (pathname, c_FILEOPEN | c_FILEREADONLY);

	if (!procfile) // the error will be cast by file i/o subsystem
		return NULL;

	heapsize = k_filesize (procfile);

	if (heapsize == 0)
		return NULL;

	if (!k_read (procfile, heap, heapsize))
		return NULL;

	heap = k_malloc (heapsize);

	// from this point on we MUST NOT be interrupted or the scheduler will mix up

        k_lock;

        process = &k_kernel.procs [k_kernel.nprocs];

        process->lasterror = c_ERRNOERROR;
	process->priotity = priority;
        process->signals = k_initsignals;

        k_kernel.nprocs++;

        if (!k_addtask (heap, process))
                goto k_suicide;

        k_unlock;

	return process;

k_suicide: // we have to clean-up our mess

        k_kernel.lastfreepid = process->pid;

        k_unlock;

	return NULL;
}

int k_killproc (struct s_proc * proc, int flags) { // flags say if child process should be killed also


        k_kernel.lastfreepid = proc->pid;

	return FALSE;
}

int k_isowner (struct s_proc * child, struct s_proc * parent) {
	
	do {

		if (child->ppid == parent->pid) // we found him
			return TRUE;

                child = k_getproc (child->ppid);

	} while (proc->ppid != 1);

	return FALSE;
}

struct s_proc * k_getproc (unsigned char pid) {

	unsigned long i;

        for (i = 0; i < k_kernel.nprocs; i++)
                if (k_kernel.procs [i].pid == pid)
                        return &k_kernel.procs [i];

        k_setlasterror (c_ERRUNKOWNPROCESS);
	return NULL;
}

unsigned char k_getlasterror () {
	

	/* by resetting the error code, functions
		do only need to post errors */

	unsigned char error;

        error = k_kernel.current->owner->lasterror;
        k_kernel.current->owner->lasterror = c_ERRNOERROR;
	
	return error;
}

void k_setlasterror (unsigned char error) {

        if (k_kernel.kernelmode) /* if an error occurs in kernel mode */
                k_kernel.lasterror = error;

        k_kernel.current->owner->lasterror = error;
}
