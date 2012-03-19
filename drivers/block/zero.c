/*
 * zero.c
 * zero-device (/dev/zero)
 * 02/14/03
 */

#include <driver.h>

/* -------------------------------------------------------*/

int zero_init () {

	return -1;
}

/* -------------------------------------------------------*/

int zero_exit () {

	return -1;
}

/* -------------------------------------------------------*/

unsigned long zero_read (void * buffer, unsigned long nbytes) {

	k_memset (buffer, nbytes, 0);

	return nbytes;
}

/* -------------------------------------------------------*/

unsigned long zero_write (void * buffer, unsigned long nbytes) {

	return 0;
}
