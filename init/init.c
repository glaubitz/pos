/*
* pOS
* init.c
* feb., 21 2001
*/

#include <pos.h>
#include <lib/cfgfile.h>
#include <lib/string.h>

int readrc () {


}

int readinittab () {


}

void error (char * var, BYTE code, WORD line) {

        char * errormsg [6] = {
                "Illegal value for var %s in rc.config (%u), %u",
                "The config file rc.config was not found, %u",
                "Unknown variable %s in rc.config (%u), %u",
                "The initialization file init.tab was not found, %u",
                "Unknown error or Kernel error, %u",
                "The program %s could not be executed, %u"
        };

        printf ("Init error\r");
        printf (errormsg [code], var, line, code);
}

void main () {

        readrc ();
        readinittab ();

}
