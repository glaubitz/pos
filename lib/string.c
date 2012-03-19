/*
* pOS
* string.c
* 10/11/05
*/

#include <pos.h>
#include <lib/string.h>

/* TODO:
 *
 * - adjust to new type definitions
 * - write strncmp routine
 * - rewrite strcmp routine
 * - write printf routine
 * - write inttostr routine
 * - clean-up and speed-up everything here
 */

void printf (char * str, ...) {


}

char * concat (char * stra, char * strb) {


	int lengtha;
	int i;

	lengtha = strlen (stra);

	i = 0;

	while (* (strb + i) != 0) {

		* (stra + lengtha) = * (strb + i);
		lengtha++;
		i++;
	}
	
	* (stra + lengtha) = 0;

	return stra;
}

int strlen (char * str) {

	int length;

	length = 0;
    
	while (* (str + length) != 0)
		length++;
	
	return length;
}

// this routine must be rewritten to comply with
// the standard C-function strcmp !, which returns
// 0 if two strings match, and nonzero values
// in other cases (don't know what right know)

int strcmp (char * stra, char * strb) {

	return -1;
}

int strncmp (char * stra, char * strb, int count) {
	
	return -1;
}

int strtoint (char * str) {

	/* UNTESTED 
	 * WORKS, IF:
	 * - '>>' is shift right
	 * - '^' is XOR
	 * - '!' is binary NOT
	 * */

	int result;

	int i, j, p;

	if ((i = strlen (str) - 1) == -1)
		return 0;

	result = 0;

	if (str [i] == '-') {
		i--;
		result = result | (!(-1 >> 1));
	}

	while (i > -1) {
		
		if (str [i] < 48 || str [i] > 58)
			return 0;
				
		p = 1;

		for (j = 0; j < i; j++)
			p = p * 10;

		result = result + (str [i] - 48) * p;
		i--;
	}

	if (result < 0)
		return !(result ^ (!(-1 >> 1)));
	else
		return result;
}

char * inttostr (int numeric) {

	return NULL;
}

