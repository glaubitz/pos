
       ### ## ##    ### ###   #######
      ###### #### #### #### ####
     ###### #### #### #### ########
    ###### #### #### ####  ########
   ### ## ##   #### ####      ####
  ###          ### ###   #######
 ### PORTABLE OPERATING SYSTEM

 README file (PRELIMENARY) for version 0.0.1

License/Copyright:
This software is covered by the GNU General Public License v2. It
may be copied free of charge, modified and redistributed under
the terms of the GPLv2 provided in the COPYING file provided
with this software.

(c) 2000-2005 Adrian Glaubitz

pOS-Homepage:
Get the latest kernel versions, kernel-ports for other hardware,
additional applications, online documents and support on the
pOS-homepage at:

		http://home.arcor.de/adi007/pOS

If you are interested in developing applications for pOS or
if you want to join the pOS-kernel development, read to the
pOS-developer documents at:

		http://home.arcor.de/adi007/pOS/dev

Bugs:
If you spot any bugs in the pOS version you are running, just drop
me a mail with a short description of the misbehavior and I'll try
to fix it. You can reach me over: adi007@arcor.de

Requirements:
- Commodore C64 or 100%-compatible (like C128 in C64-mode)
- Commodore 1541 or 100%-compatible (like the 1541II or the 1571 in 1541-mode)
- a blank 5-1/4" floppy disk
- an X1541-PC-Transfer Cable
- cbm4linux, Star Commander, X1541 or equivalent 
- installed and working cc65 compiler (www.cc65.org)
- installed and working GNU make (ftp.gnu.org)

pOS might also work with other hardware configurations (like the
Commodore 1581 disk drive) but that has not been tested (yet).

If you don't have a C64 anymore (shame on you !) or you don't have
the possibility to copy the files onto a 1541 floppy disk, you
can also tryout pOS on a emulator like ViCE which is available
for several platforms.

Compiling:

- Type 'make' in the root folder of the kernel source,
  the kernel and it's components are now being compiled
  which means they are translated into machine code.
  Please see the text file compile.txt in same folder
  like this readme-file for help on compiling and
  compiling erros.

- Copy the following files to a 1541 disk (using Star
  Commander or anything similar):

	BOOT
	KERNEL
	INIT
	INIT.TAB
	RC.CFG
	SH

- You can type "make clean" to remove all program files again
  after you have copied the files to your C64. But you will
  have to compile the kernel again in case you need the
  kernel files again.

Using:

1. Starting

- Turn on C64 and 1541 disk unit.

- Insert the created disk into your 1541.

- At the BASIC prompt type:
	LOAD"BOOT",8

- Wait until the computer displays "READY."

- Now type:
	"RUN"

- The system should now be booting and display certain messages.
  If there occur any messages with "[FAILED]" or the computer
  freezes please reset your computer and your floppy disk and
  try again to load pOS. If the problem persists you might want
  to check if you forgot any of the required files or check
  out the pOS-homepage to receive support with e-mail contact.

- After that, the computer will prompt with:

	"/: _"

  This means you are currently in the root-
  directory ("/"). pOS is now ready for work.

2. Shutting down

- Just turn off C64 and 1541, there is no program
  for shutting down yet.

3. Loading programs

- At the prompt "/: _" type the name of the program
  you'd like to load, the computer will then try to
  load and execute it.
	For example:
		"/: test_<RETURN>"

	Will try load the program "test" and execute it.

	If the program does not exist on the current disk
	the shell will prompt with an error:
		"unknown command/filename"
