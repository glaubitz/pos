/*
 * mem.c
 * memory management
 * 01/15/05
 */

#include <kernel.h>
#include <ASM/mem.h>

/*
* now, here comes the concept, still to be improved (according to c64 memmap):
* to allocate memory, malloc is used, every size of memory can be
* allocated, but it will always use at least 4 bytes of memory for
* this reason:
* every block of memory allocated, has two additional control bytes:
* OFFSET        Usage
* 00h           PID (process id of owner, necessary for memory
*               freeing when process is killed)
* 01h           size of this block of memory (to determine next block)
* 03h           memory data here, size at least 1 byte, at maximum c_MAX_MEM - 3 (5 on 32Bit machines) ctrl bytes
*
* ~problem:~ not really, system can reboot
* when much memory is allocated in small chunks, and freed, the memory
* will fragment a lot
*/

/*

BUGS:

  - malloc () does not consider reserved blocks when memory is LOW (save available mem to atomic ?)
	--> has been added, but it's not been verified
  - remalloc () can only increase memory, but cannot shrink memory blocks->done
  - free () does not know anything about reserved blocks, see also comments in free () code
        --> has been added, but not verified yet
  - free () cannot yet contingate possible free blocks before the freed block to it
	(needs a macro to get PREVIOUS memory blocks header)

  - meminit () does not correctly set the memory blocks sizes as it does
    not know the sizes of the hardware reserved memory blocks inbetween
	->fixed

  also:
	- optimize the code (making it more readible also) !!
	- what about wild pointers (return special struct instead of pointer ?)
	
*/

void k_memory_init () {

        void * pmem;
        struct s_mem * pmembinfo;

        pmem = c_MEMFREESTART;

        while (1) {

		pmembinfo = (struct s_mem *) pmem;
		pmembinfo->pid = c_PIDFREE;
		pmembinfo->size = m_MEMGETCURSIZE(pmembinfo) - sizeof (struct s_mem);
		pmem = m_MEMGETNEXTFREE(pmem);


		if (!pmem)
			break;

                /*

		--COMMENT OBSOLETE BUT KEPT FOR DOCUMENTATION--

                the following assumes that the current hardware free memory
                block and the next are directly next to each other
                which is NONSENSE as there IS acutally an unusuable
                hardware-reserved memory block inbetween them

                probably we need a GET_NEXTRESERVED(pmem) or we BETTER
                develop a system which stores all free areas AND
                their sizes in a header-file which is read by mem.c

		pmembinfo->size = (pmem - pmembinfo) - sizeof (struct s_mem);

                */
        }
}

void * k_memory_alloc (unsigned long size, unsigned long reserve) {

	void * pmem;
	int usereserved;

	// if the requsted size is not available as free block
	// we might find it in a reserved block
	
        use_reserved = (k_getfreemem () < size + reserve);

        pmem = c_MEMFREESTART;

	do {

		/* we need a block that is:
			- free (pid = 254) or reserved (pid = 255) when memory is low
			- is larger than or equal to size + reserve + sizeof (struct s_mem)
				(we need space for a second struct s_mem struct when reserving extra memory)
		*/
		
		switch (((struct s_mem *) pmem)->pid) {

                        case c_PIDRESERVED:
				// use reserved memory for allocating new memory also ?
				
				if (!usereserved)
					break; // no, find a free block
			
                        case c_PIDFREE:
			
                                ((struct s_mem *) pmem)->pid = k_kernel.current->owner->pid;

				if (reserve) {

                                        ((struct s_mem *) pmem + size + sizeof (struct s_mem))->pid = c_PIDRESERVED; // mark next block as reserved for remalloc
					((struct s_mem *) pmem + size + sizeof (struct s_mem))->size = reserve; // store size of reserved block
				}

				if (((struct s_mem *) pmem)->size >= size + reserve +
					(reserve ? sizeof (struct s_mem) : 0) + sizeof(struct s_mem)) {

                                        ((struct s_mem *) pmem + size + reserve + (reserve ? sizeof (struct s_mem) : 0))->pid = c_PIDFREE; // new size of empty block
					((struct s_mem *) pmem + size + reserve + (reserve ? sizeof (struct s_mem) : 0))->size -=
					(size + reserve + (reserve ? sizeof (struct s_mem) : 0) + sizeof(struct s_mem));
				}

				(struct s_mem *) pmem->size = size;

                                return (pmem + sizeof (struct s_mem));
			default:
				break;
		} // switch

		pmem += ((struct s_mem *) pmem)->size + sizeof (struct s_mem); // advance to next memory block

                if (!m_MEMBLOCKAVAIL(pmem))
			pmem = m_MEMGETNEXTFREE(pmem); // call a macro that determines next free memory block
					
		if (!pmem)
			break;


	} while (1);


        k_setlasterror (c_ERROUTOFMEM);

	return NULL;
}

void k_memory_set (void * object, unsigned long size, unsigned char value) {

        m_memset(object, size, value); // assembler macro memset, faster
}

void k_memory_copy (void * source, void * target, unsigned long size) {

	m_memcpy(source, target, size); // assembler macro memset, faster
}

int k_memory_free (void * object) {

	struct s_mem * thismemblock, reservedmemblock;

	thismemblock = (struct s_mem *) (object - sizeof (struct s_mem));
	reservedmemblock = (struct s_mem *) (object + thismemblock->size);

        if (thismemblock->pid != k_kernel.current->owner->pid) {
                k_setlasterror (c_ERRACCESSVIOLATION);
		return NULL;
	}

        thismemblock->pid = c_PIDFREE; // free

        if (reservedmemblock->pid == c_PIDRESERVED) {
                thismemblock->size += reservedmemblock->size + sizeof (struct s_mem);
                if ((struct s_mem *) (reservedmemblock + reservedmemblock->size + sizeof (struct s_mem))->pid == c_PIDFREE)
                        thismemblock->size += ((struct s_mem *) (reservedmemblock + reservedmemblock->size + sizeof (struct s_mem))->size);
        }
        else
                if ((struct s_mem *) (thismemblock + thismemblock->size + sizeof (struct s_mem))->pid == c_PIDFREE)
                        thismemblock->size += (struct s_mem *) (thismemblock + thismemblock->size + sizeof (struct s_mem))->size;

	// we should not simply free the memory block
	// but also check if it is sorrounded by other
	// free memory blocks, then we could concatenate
	// them to one big block

	return TRUE;
}

void * k_memory_realloc (void * object, unsigned long size) {

	struct s_mem * thismemblock, reservedmemblock;

	thismemblock = (struct s_mem *) (object - sizeof (struct s_mem));
	reservedmemblock = (struct s_mem *) (object + thismemblock->size);

        if (thismemblock->pid != k_kernel.current->owner->pid) {
                k_setlasterror (c_ERRACCESSVIOLATION);
		return NULL;
	}

	if (thismemblock->size == size) // if the current size is requested
		return object;


	if (size > thismemblock->size) { // increase block size

                if (reservedmemblock->pid != c_PIDRESERVED ||
			reservedmemblock->size < (size - thismemblock->size)) {

                        k_setlasterror (c_ERROUTOFMEM);
			return NULL;
		}

                (thismemblock + sizeof (struct s_mem) + size)->pid = c_PIDRESERVED;
		(thismemblock + sizeof (struct s_mem) + size)->size = reservedmemblock->size - (size - thismemblock->size);

		thismemblock->size = size;
	}
	else { // decrease block size (size < thismemblock->size)

                (thismemblock + sizeof (struct s_mem) + size)->pid = c_PIDRESERVED;
		(thismemblock + sizeof (struct s_mem) + size)->size = reservedmemblock->size + (thismemblock->size - size);

		thismemblock->size = size;
	}

	return object;
}

unsigned long k_memory_getfree () {

	void * pmem;
        unsigned long free;

        pmem = c_MEMFREE_START;
	free = 0;
        pmem = m_MEM_GETNEXTFREE(pmem);

	do {
                if ((struct s_mem *) pmem->pid = c_PIDFREE)
			free += (struct s_mem *)pmem->size;
		
		pmem += pmem + (struct s_mem *)pmem->size + sizeof (struct s_mem);
                if (!m_MEM_BLOCKAVAIL(pmem))
                        pmem = m_MEM_GETNEXTFREE(pmem);
	} while (pmem);

	return free;
}

unsigned long k_memory_getreserved (struct s_mem * pmembinfo) {

	void * pmem;
        unsigned long reserved;

	if (pmembinfo) {
                if (* (pmembinfo + pmembinfo->size + sizeof (struct s_mem))->pid == c_PIDRESERVED)
			return * (pmembinfo + pmembinfo->size + sizeof (struct s_mem))->size;
		else
			return 0;
	}
	else {
                pmem = c_MEMFREE_START;
		reserved = 0;
                pmem = m_MEMGETNEXTFREE(pmem);
		do {
			reserved += (struct s_mem *) pmem->reserved;
			pmem += pmem + (struct s_mem *)pmem->size + sizeof (struct s_mem);
                        if (!m_MEMBLOCKAVAIL(pmem))
                                pmem = m_MEMGETNEXTFREE(pmem);
		} while (pmem);

		return reserved;
	}
}

unsigned long k_memory_getused (struct s_mem * pmembinfo) {

	void * pmem;
        unsigned long used;

	if (pmembinfo)
                return pmembinfo->size;
	else {
                pmem = c_MEMFREESTART;
		used = 0;
                pmem = m_MEMGETNEXTFREE(pmem);
		do {
			used += (struct s_mem *) pmem->size + sizeof (struct s_mem);
			pmem += pmem + (struct s_mem *)pmem->size + sizeof (struct s_mem);
                        if (!m_MEMBLOCKAVAIL(pmem))
                                pmem = m_MEMGETNEXTFREE(pmem);
		} while (pmem);

		return used;
	}
}
