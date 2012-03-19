/*
* const.h
* 05/03/05
* system wide constants
*/

#ifndef __CONST_H
#define __CONST_H

/* process messages */

#define c_ERRNOERROR		0x00
#define c_ERROUTOFMEM		0x01
#define c_ERRACCESSVIOLATION	0x02
#define c_ERRFILENOTFOUND	0x03
#define c_ERRFILENOTOPEN	0x04
#define c_ERRILLEGALPATHNAME	0x05
#define c_ERRFILEREADONLY	0x06
#define c_ERRILLEGALPARAMETER	0x07
#define c_ERRUNKNOWNPROCESS	0x08
#define c_ERRUNKNOWNTASK	0x09
#define c_ERRMAXNPROCSREACHED	0x0a
#define c_ERRMAXNTASKSREACHED	0x0b
#define c_ERRDEVICEBUSY		0x0c // *
#define c_ERRUNKNOWNSIGNAL	0x0d
#define c_ERRUNKNOWNDEVICE	0x0e // * do we still need these two ?
#define c_ERRMAXNMNTLISTENTRYREACHED 0x0f

/* strings */

#define c_STRKERNELVERSION	"0.0.1"
#define c_STRKERNELCOPYRIGHT	"(C) A.GLAUBITZ 2000-2005"

/* device types */

#define c_DEVICECHAR		0x00
#define c_DEVICEBLOCK		0x01

/* file-open modes */

#define c_FFLAGOPEN		%00000001
#define c_FFLAGCREATE		%00000010
#define c_FFLAGOVERWRITE	%00000011
#define c_FFLAGREADONLY		%01000000
#define c_FFLAGREADWRITE	%10000000
#define c_FFLAGWRITEONLY	%11000000

/* filesystem types */

#define c_FSUNDEFINED		0x00
#define c_FSPFS			0x02

/* other */

#define c_PIDFREE		254
#define c_PIDRESERVED		255
#define c_PROCMINTTL		50

/* contraints */

#define c_MAXNDEVICE		4
#define c_MAXNPROCS		10
#define c_MAXNTASKS		20
#define c_MAXNMNTLISTENTRY	4
#define c_MAXNOPENFILES		20
#define c_MAXPATHLENGTH		64
#define c_MAXDIRENTRY		32
#endif
