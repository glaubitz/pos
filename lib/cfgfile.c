/*
* Configuration File Library - pOS Version
* (c) Adrian Glaubitz - adi007@germanynet.de
* 03/27/02
* cfgfile.c - function defitions
*/

#include <pos.h>
#include <lib/string.h>
#include <lib/cfgfile.h>

#define MAX_STRING_LENGTH	64
#define CFGFILE_MSG_INIT	"Configuration File Library 0.0.2  - 03/27/2002 - (c) 2001-2002 Adrian Glaubitz"

static void (* printmsg) (char * message);
void parseerr (short err, unsigned long line, char * filename);
static FILEINFO * InputFile, * OutputFile;
static unsigned short nparseerr;

int InitLibrary (void (* pprintproc) (char * message)) {

	if (!pprintproc)
		return -1;
	
	printmsg = pprintproc;

	printmsg (CFGFILE_MSG_INIT);
	printmsg (""); // making an empty line like in the good old BASIC days :)

	return 0;
}

void ClearAllData (CFGFILE * pConfigFile) {

	unsigned isection, ientry;

	printmsg ("Removing all file data of current config-file from memory...");

	for (isection = 0; isection < pConfigFile->SectionCount; isection++) {
		for (ientry = 0; ientry < pConfigFile->aConfigSections [isection].EntryCount; ientry++) {
			free (pConfigFile->aConfigSections [isection].aConfigEntries [ientry].Name);
			free (pConfigFile->aConfigSections [isection].aConfigEntries [ientry].Value);
		}

		free (pConfigFile->aConfigSections [isection].aConfigEntries);
		free (pConfigFile->aConfigSections [isection].Name);
	}

	free (pConfigFile->aConfigSections);
	pConfigFile->SectionCount = 0;
	pConfigFile->aConfigSections = NULL;

	printmsg ("Removing done");

}

unsigned short GetNParseErrors () {

	return nparseerr;
}

unsigned long WriteConfigFile (char * filename, CFGFILE * pConfigFile) { // returns filesize
/*
	FILEINFO * OutputFile, * InputFile;
	unsigned short iSection, iEntry;
	unsigned long i;
	char buffer [MAX_STRING_LENGTH + 1];
	char c;

	if (!(OutputFile = fopen (filename, "r+"))) // a+ !!
		return 0;

	printmsg ("Writing config file");

	for (iSection = 0; iSection < pConfigFile->SectionCount; iSection++) {

		while (1) {

			c = fgetc (OutputFile);

			switch (c) {

			case '[':
				// check section name and rewrite section on match
				for (i = 0; i < MAX_STRING_LENGTH &&
					(c = fgetc (InputFile)) != EOF; i++) {

					if (c == ']')
						break;

					switch (c) {

					case '#':
						parseerr (0); // comment in entry or section name
						goto nextline; // what to do here ???
					
					case '\n':
						parseerr (1); // unexpected end of line
						goto nextline; // s.a.
					
					default:
						if (isalnum (c)) {
							buffer [i] = c;
							break;
						}
						else {
							parseerr (2); // unexpected character found
							goto nextline; // s.a.
						}
					}
				}
						
				break;

			case EOF:
				// append new section
				break;

			default:
				// simply fetch next char
				break;
			}
		}
	}
*/
	return 0;
}

CFGENTRY * SetEntry (CFGFILE * pConfigFile, char * SectionName, char * EntryName, char * EntryValue) {

	/* if entry does not exist, it will be created automatically */

	CFGSECTION * pConfigSection;
	unsigned short i;

	i = 0;

	while (1) {

		if (i == pConfigFile->SectionCount)
			if (! (pConfigSection = AddSection (pConfigFile, SectionName)))
				return NULL;
			else
				break;


		if (strcmp (pConfigFile->aConfigSections [i].Name, SectionName) == 0) {

			pConfigSection = &pConfigFile->aConfigSections [i];
			break;
		}

		i++;
	}

	i = 0;

	while (1) {

		if (i == pConfigSection->EntryCount) {

			pConfigSection->EntryCount++;

			pConfigSection->aConfigEntries = realloc (pConfigSection->aConfigEntries,
										pConfigSection->EntryCount * sizeof (CFGENTRY));

			pConfigSection->aConfigEntries [i].Name = malloc (strlen (EntryName) + 1);
			strcpy (pConfigSection->aConfigEntries [i].Name, EntryName);
			
			pConfigSection->aConfigEntries [i].Value = NULL; // to make further calls to realloc work
			break;
		}


		if (strcmp (pConfigSection->aConfigEntries [i].Name, EntryName) == 0)
			break;

		i++;
	}

	pConfigSection->aConfigEntries [i].Value = realloc (pConfigSection->aConfigEntries [i].Value,
											strlen (EntryValue) + 1);

	strcpy (pConfigSection->aConfigEntries [i].Value, EntryValue);

	return &pConfigSection->aConfigEntries [i];
}

CFGSECTION * AddSection (CFGFILE * pConfigFile, char * SectionName) {

//	CFGSECTION * pConfigSection;

	if (pConfigFile) {

		pConfigFile->SectionCount++;
		pConfigFile->aConfigSections = realloc (pConfigFile->aConfigSections,
								pConfigFile->SectionCount * sizeof (CFGSECTION));

		pConfigFile->aConfigSections [pConfigFile->SectionCount - 1].Name = malloc (strlen (SectionName) + 1);
		strcpy (pConfigFile->aConfigSections [pConfigFile->SectionCount - 1].Name, SectionName);

		pConfigFile->aConfigSections [pConfigFile->SectionCount - 1].EntryCount = 0;
		pConfigFile->aConfigSections [pConfigFile->SectionCount - 1].aConfigEntries = NULL;

		return &pConfigFile->aConfigSections [pConfigFile->SectionCount - 1];
	}
	else
		return NULL;
}

unsigned short RemoveEntry (CFGFILE * pConfigFile, char * SectionName, char * EntryName) { // returns new entry count
	
	return 0;
}

unsigned short RemoveSection (CFGFILE * pConfigFile, char * SectionName) { // returns new section count

	unsigned short i, c;

	i = 0;

	while (1) {

		if (strcmp (pConfigFile->aConfigSections [i].Name, SectionName) == 0) {
			c = i;
			break;
		}

		i++;

		if (i == pConfigFile->SectionCount)
			return i;
	}

	for (i = 0; i < pConfigFile->aConfigSections [c].EntryCount; i++) {

		free (pConfigFile->aConfigSections [c].aConfigEntries [i].Name);
		free (pConfigFile->aConfigSections [c].aConfigEntries [i].Value);
	}

	free (pConfigFile->aConfigSections [c].aConfigEntries);

	pConfigFile->aConfigSections [c] = pConfigFile->aConfigSections [pConfigFile->SectionCount - 1];

	pConfigFile->SectionCount--;

	realloc (pConfigFile->aConfigSections, pConfigFile->SectionCount * sizeof (CFGSECTION));

	return pConfigFile->SectionCount;
}

CFGFILE * ReadConfigFile (char * filename, CFGFILE * pConfigFile) {

	unsigned long line;
	unsigned long i;
	unsigned long ivalue;
	char c;
	char buffer [MAX_STRING_LENGTH + 1];
	//FILE * InputFile;
	CFGSECTION * pCurrentSection;

	if (!(InputFile = fopen (filename, "r")))
		return NULL;

	printmsg ("Reading config file...");

	line = 0;
	nparseerr = 0;
	pCurrentSection = NULL;
	buffer [MAX_STRING_LENGTH] = '\0';

nextline:

	line++;

	while (1) {

		c = fgetc (InputFile);

		switch (c) {

		case EOF:
			goto readingdone;

		case ' ':
			break;

		case '\n':
			line++;
			break;

		case '#':
			while ((c = fgetc (InputFile)) != EOF) {
				if (c == '\n') {

					line++;
					break;
				}
			}
			break;

		case ']':
			parseerr (7, line, filename);
			goto nextline;

		case '[':

			pCurrentSection = NULL;

			for (i = 0; i < MAX_STRING_LENGTH &&
				(c = fgetc (InputFile)) != EOF; i++) {

				if (c == ']')
					break;

				switch (c) {

				case '#':
					parseerr (0, line, filename); // comment in entry or section name
					goto nextline;
				case '\n':
					parseerr (1, line, filename); // unexpected end of line
					goto nextline;
				default:
					if (isalnum (c) || c == ' ') {
						buffer [i] = c;
						break;
					}
					else {
						parseerr (2, line, filename); // unexpected character found
						goto nextline;
					}
				}
			}

			if (c == EOF) {
				parseerr (6, line, filename); // unexpected end of file
				goto readingdone;
			}

			if (i == 0) {
				parseerr (5, line, filename); // string has zero length
				goto nextline;
			}

			if (c != ']') {
				parseerr (4, line, filename); // string too long
				goto nextline;
			}

			buffer [i] = '\0';

			pCurrentSection = AddSection (pConfigFile, buffer);

			break; // case ']'

			default: // parse next entry and its value

				if (!isalnum (c)) {
					parseerr (2, line, filename); // unexpected character found
					goto nextline;
				}

				if (!pCurrentSection) {

					parseerr (3, line, filename); // entry without section
					goto nextline;
				}

				i = 0;
				ivalue = 0;

				do {

					switch (c) {

					//case ' ':
					//	break;

					case '#':
						parseerr (0, line, filename); // comment in entry or section name
						goto nextline;

					case '=':

						if (i == 0) {
							parseerr (5, line, filename);
							goto nextline;
						}

						buffer [i] = '\0';
						ivalue = i + 1;

						break;

					default:
						if (isalnum (c) || c == ' ' || c == '.' || c == ',') {
							buffer [i] = c;
							break;
						}
						else {
							parseerr (2, line, filename); // unexpected character found
							goto nextline;
						}
					}

					i++;

				} while (i < MAX_STRING_LENGTH && (c = fgetc (InputFile)) != EOF && c != '\n' && c != ';');

				if (c == EOF) {
					parseerr (6, line, filename); // EOF
					goto readingdone;
				}

				if (c == '\n') {
					parseerr (9, line, filename); // ';' expected
					goto nextline;
				}

				if (ivalue == 0) {
					parseerr (8, line, filename);
					goto nextline;
				}

				if (i == ivalue) {
					parseerr (5, line, filename);
					goto nextline;
				}

				buffer [i] = '\0';

				SetEntry (pConfigFile, pCurrentSection->Name, buffer, buffer + ivalue);
				
				goto nextline;

		} // switch

	} // while (1)

readingdone:

	printmsg ("Reading done");

	return pConfigFile;
}

char * ReadEntry (CFGFILE * pConfigFile, char * SectionName, char * EntryName) {

	return NULL;
}

void parseerr (short err, unsigned long line, char * filename) {

	char buffer [64];
	char c;

	nparseerr++;
	
	switch (err) {

	case 0:
		sprintf (buffer, "parse error (%d): comment in entry or section-name",
						line);
		break;
	case 1:
		sprintf (buffer, "parse error (%d): unexpected end of line",
						line);
		break;
	case 2:
		sprintf (buffer, "parse error (%d): unexpected character found",
						line);
		break;
	case 3:
		sprintf (buffer, "parse error (%d): entry without section",
						line);
		break;
	case 4:
		sprintf (buffer, "parse error (%d): string too long",
						line);
		break;
	case 5:
		sprintf	(buffer, "parse error (%d): string has zero length",
						line);
		break;
	case 6:
		sprintf (buffer, "parse error (%d): unexpected end of file",
						line);
	
		printmsg (buffer);
		return;
	case 7:
		sprintf (buffer, "parse error (%d): closing ']' without opening '['",
						line);
		break;
	case 8:
		sprintf (buffer, "parse error (%d): '=' expected",
						line);
		break;

	case 9:
		sprintf (buffer, "parse error (%d): ';' expected before end of line",
						line);
		break;
	default:
		sprintf (buffer, "parse error (%d): unknown error",
					line);
	}

	printmsg (buffer);

	while ((c = fgetc (InputFile)) != EOF) {
		if (c == '\n')
			break;
	}
	
}
