/*
 * version.c
 * system version control
 * 02/24/03
 */

#include <kernel.h>

unsigned short k_getver () {

	return (c_KRNLVERSIONMAJOR << 4) | c_KERNELVERSIONMINOR;
}

int k_checkexechdr (void * exechdr) {

	// NO EXE-FORMAT YET !!
	
	return 0;
}
