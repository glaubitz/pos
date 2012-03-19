/*
 * pOS
 * console.c - console functions
 * 01/15/05
 */

#include <kernel.h>

int k_console_redirect (char * filename) {

	struct s_file * console;

	k_file_close (k_kernel.console);

	console = k_file_open (pathname, c_FFLAGOPEN | c_FFLAGREADWRITE);

	if (!console)
		return FALSE;
	else {
		k_kernel.console = console;
		return TRUE;
	}
}

int k_console_put (char * s) {

	if (strlen (s) > 255)
		return FALSE;
	
	k_write (k_kernel.console, s, strlen (s));

	return TRUE;
}

int k_console_get (char * s, unsigned char bufsize) {


	k_read (k_kernel.console, s, bufsize);

	return TRUE;
}

