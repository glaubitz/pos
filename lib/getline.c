#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFERSIZE 1024

static char * gpbuf = NULL; /* Globaler Puffer (statisch) */
static int offset; /* aktuelles Offset im globalen Puffer (statisch) */
static int bufsize = BUFFERSIZE; /* aktuelle Groesse des globalen Puffers (statisch) */

char * getline (int fd) {

	char * pbuf; /* Zeiger auf Line-Buffer */
	int readcount = 0; /* wie viele Zeichen beim letzten Mal gelesen wurden */
	int i = offset; /* Laufvariable fuer eine Zeile */

	/* der globale Puffer muss nur einmal pro Datei angelegt werden */

	if (gpbuf == NULL)
		gpbuf = malloc (BUFFERSIZE);

	do {

		/* Versuche zunaechst den globalen Puffer mit Bytes zu fuellen */
		
		readcount = read (fd, (char*) (gpbuf + offset), bufsize - offset);
		offset = readcount;

		/* wenn der ganze Puffer durch das Einlesen gefuellt wurde, verdopple seine Groesse */
		
		if (readcount == bufsize)
			gpbuf = realloc (gpbuf, bufsize * 2);

		/* bestimme eine Zeile durch Suchen von '\n' */

		while (* (gpbuf + i) != '\n' && i < offset)
			i++;

		/* wenn kein Newline gefunden, weiter einlesen versuchen */
		if (i == readcount)
			continue;

		/* wenn gefunden, dann Zeile bis Newline in Ergebnispuffer kopieren */
		else {
			pbuf = malloc (i);
			memcpy (pbuf, gpbuf, i);
			memmove (gpbuf, gpbuf + i + 1, bufsize - i - 1);
			offset = readcount - i;
			readcount = 0;
			return pbuf;
		}


	/* wiederhole Schleife solange, bis nix aus der Datei mehr eingelesen werden kann */

	} while (readcount > 0);

	/* gib den globalen Puffer wieder frei */

	free (gpbuf);

	return NULL;
}


int main () {

	int fd;
	char * buffer;

	fd = open ("test.txt", O_RDONLY);

	buffer = getline (fd);
	printf ("%s\n", buffer);
	buffer = getline (fd);
	printf ("%s\n", buffer);
	return 0;
}
