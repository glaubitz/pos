/*
* Configuration File Library - pOS Version
* (c) Adrian Glaubitz - adi007@germanynet.de
* 03/27/02
* cfgfile.h - structs, declarations for the library
*/

typedef struct __CFGENTRY {

	char * Name;
	char * Value;

} CFGENTRY;

typedef struct __CFGSECTION {

	char * Name;
	CFGENTRY * aConfigEntries;
	unsigned short EntryCount;

} CFGSECTION;

typedef struct __CFGFILE {

	FILE * ConfigFile;
	CFGSECTION * aConfigSections;
	unsigned short SectionCount;

} CFGFILE;

int InitLibrary (void (* pprintproc) (char * message));

void ClearAllData (CFGFILE * pConfigFile);

unsigned short GetNParseErrors ();

CFGENTRY * SetEntry (CFGFILE * pConfigFile, char * SectionName,
						char * EntryName, char * EntryValue);

CFGSECTION * AddSection (CFGFILE * pConfigFile, char * SectionName);

unsigned short RemoveEntry (CFGFILE * pConfigFile, char * SectionName, char * EntryName);

unsigned short RemoveSection (CFGFILE * pConfigFile, char * SectionName);

char * ReadEntry (CFGFILE * pConfigFile, char * SectionName, char * EntryName);

CFGFILE * ReadConfigFile (char * filename, CFGFILE * pConfigFile);

unsigned long WriteConfigFile (char * filename, CFGFILE * pConfigFile);