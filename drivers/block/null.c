/*
 * null.c
 * null-device (/dev/null)
 * 02/14/03
 */

#include <driver.h>

/* -------------------------------------------------------*/

int null_init () {

	return -1;
}

/* -------------------------------------------------------*/

int null_exit () {

	return -1;
}

/* -------------------------------------------------------*/

unsigned long null_read (void * buffer, unsigned long nbytes) {

	return 0;
}

/* -------------------------------------------------------*/

unsigned long null_write (void * buffer, unsigned long nbytes) {

	return nbytes;
}
