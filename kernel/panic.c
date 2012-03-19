/*
* pOS
* panic.c
* 01/15/05
*/

#include <kernel.h>

void k_panic_panic (char * panictxt) {

        k_lock;

	//char * panictxt = "Kernel panic, system halted";

	k_conput ("Kernel panic - System halted: ");
	k_conput (panictxt);

	// filesystem sync (also missing in dcache.c) !!
	
	while (1)
		;
}
