/*
 * shell.c - Shell for pOS
 * BUGS:
 *
 * - to be adapted to pOS (written originally for Linux for debug)
 * - does not allow backgrounding
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_COMMANDLENGTH 64
#define MAX_COMMANDCOUNT 64
#define MAX_ARGCOUNT 10
#define MAX_INPUTBUFSIZE 1024
#define MAX_HISTCOUNT 10
#define MAX_PATHLENGTH 256

#define TRUE -1
#define FALSE 0

/* define __DEBUG__ here to get extended debugging output */

//#define __DEBUG__

/* define a struct to represent a shell-command in memory */

struct s_command {

	char * name; /* command-name, i.e. name of (binary) executable */
	char * argv [MAX_ARGCOUNT]; /* array of char-pointers to store command arguments */
	int argn; /* number */
	char * inputfile; /* name of input-file to redirect stdin to */
	char * outputfile; /* name of output-file to redirect stdout to */
	int pipe; /* are we piping ?!?! */
};

/* function prototypes */

void prompt (char curdir [MAX_PATHLENGTH]); /* display a prompt onto the screen */
void readcommand (char * inputbuffer, struct s_command cmdbuffer [MAX_COMMANDCOUNT], int * cmdcount); /* parse cmdline into cmdbuffer struct */
void execcommand (struct s_command cmdbuffer [MAX_COMMANDCOUNT], int cmdcount); /* execute command buffer */
void puthistory (char * inputbuffer, int bufsize, char * histbuffer [], int * histcount); /* put input buffer onto history buffer */
void listhistory (char ** histbuffer, int histcount); /* list history contents */
void freecmdbuffer (struct s_command cmdbuffer [MAX_COMMANDCOUNT], int cmdcount); /* free cmdbuffer, done after execcommand */

int main () {

	char inputbuffer [MAX_INPUTBUFSIZE]; /* pointer to buffer containing string */
	struct s_command cmdbuffer [MAX_COMMANDCOUNT]; /* buffer containing single commandline-parts */
	char * histbuffer [MAX_HISTCOUNT]; /* history buffer */
	int histcount = 0; /* current number of items on buffer */
	int bufsize = MAX_INPUTBUFSIZE; /* size of the input buffer, determined by getline */
	char curdir [MAX_PATHLENGTH]; /* current working directory */
	int cmdcount; /* number of commands in current command line */
	int i, j; /* loop indices */

	/* main program-loop */

	while (1) {

		getcwd (curdir, MAX_PATHLENGTH); /* get current working directory */
		prompt (curdir); /* display prompt */

		i = 0; /* set buffer index to 0 */

		while ((inputbuffer [i++] = getchar ()) != '\n' && (i < MAX_INPUTBUFSIZE))
			; /* fetch bytes from keyboard into input buffer */

		inputbuffer [i-1] = '\0'; /* terminate string */

		if (i == 1) /* if command length is 1 only, try to read new command (return was pressed only) */
			continue;
			
		readcommand (inputbuffer, cmdbuffer, &cmdcount); /* try to interpret command */


		/* test input buffer for built-in commands */
		if (inputbuffer [0] == '!') { /* history */

			if (inputbuffer [1] == '!') { /* '!!' means execute last command again */
#ifdef __DEBUG__
				printf ("DEBUG: Copying histbuffer [0] into inputbuffer: %s\n", histbuffer [0]);
#endif
				strcpy (inputbuffer, histbuffer [0]);
				readcommand (inputbuffer, cmdbuffer, &cmdcount);
			} else {
				j = atoi (&(inputbuffer [1])); /* try to interpret history index behind '!' */
				if (j <= histcount && j >= 0) {
#ifdef __DEBUG__
					printf ("DEBUG: Copying histbuffer [%d] into inputbuffer: %s\n", j, histbuffer [j - 1]);
#endif
					strcpy (inputbuffer, histbuffer [j - 1]);
					readcommand (inputbuffer, cmdbuffer, &cmdcount);
				} else {
					printf ("DEBUG: Error: Invalid history selection: %d\n", j);
					continue;
				}
			}
		} /* history = list history contents */
		if (strcmp (cmdbuffer [0].name, "history") == 0) {
			listhistory (histbuffer, histcount);
			continue;
		} else if (strcmp (cmdbuffer [0].name, "exit") == 0) /* exit = leave shell */
			return 0;
		else if (strcmp (cmdbuffer [0].name, "cd") == 0) { /* cd = change directory */
#ifdef __DEBUG__
			printf ("DEBUG: About to change into directory \"%s\"\n", cmdbuffer [0].argv [1]);

#endif
			if (!(chdir (cmdbuffer [0].argv [1])  == 0)) {
				j = errno; /* errno might be set by printf, too */
				printf ("Cannot change directory, error %d\n", j);
			}
		}
		else
			execcommand (cmdbuffer, cmdcount); /* if it does not fit into any pattern, try to execute the command */
			

		/* free commandbuffer, to be ready for next call of readcommand */
		freecmdbuffer (cmdbuffer, cmdcount);
		/* put command onto history buffer */
		puthistory (inputbuffer, bufsize, histbuffer, &histcount);
	}
	
	return 0;
}

/* prompt - output a prompt */

void prompt (char curdir [MAX_PATHLENGTH]) {
	
	printf ("%s $: ", curdir);
}

/* readcommand - read and parse command line and put everything onto command buffer
 */

void readcommand (char * inputbuffer, struct s_command cmdbuffer [MAX_COMMANDCOUNT], int * cmdcount) {

	char tmpbuf [MAX_INPUTBUFSIZE]; /* temporary buffer for string copying, same size as input-buffer */
	int i = 0, j = 0, cmd, cmdindex = 0; /* i, j = loop indices, cmd = command index, cmdindex = index in inputbuffer */

	for (cmd = 0; cmd < MAX_COMMANDCOUNT; cmd++) {

		i = cmdindex;
		j = 0;
#ifdef __DEBUG__
		printf ("DEBUG: I'm here, about to do next command !\n");
		printf ("DEBUG: i: %d, cmdindex: %d, cmd %d, j: %d\n", i, cmdindex, cmd, j);
#endif

		cmdbuffer [cmd].pipe = FALSE; /* reset PIPE status */

		/* try to parse command name, i.e. "ls" */
		while (inputbuffer [i] != ' ' && inputbuffer [i] != '\0') {
			tmpbuf [j] = inputbuffer [i];
			i++;
			j++;
		}
		tmpbuf [j] = '\0'; /* terminate temp-string */
		cmdbuffer [cmd].name = malloc (MAX_COMMANDLENGTH);
		cmdbuffer [cmd].argn = 0; /* reset number of arguments */
		strcpy (cmdbuffer [cmd].name, tmpbuf);

//		if (inputbuffer [i] == '|') {
//			cmdbuffer [cmd].pipe = TRUE;
//			break;
//		}
//

		/* now try to parse arguments, these include the command-name itself (argv [0]) */
		for (i = cmdindex;; i++) {
	
			j = 0;
			tmpbuf [0] = '\0';		

			while (inputbuffer [i] != ' ' && inputbuffer [i] != '\0' && inputbuffer [i] != '|'
					&& inputbuffer [i] != '<' && inputbuffer [i] != '>') {
				tmpbuf [j] = inputbuffer [i];
				i++;
				j++;
			}

#ifdef __DEBUG__
			printf ("DEBUG: i: %d, cmdindex: %d, cmd %d, j: %d\n", i, cmdindex, cmd, j);
#endif

			/* a pipe-sign '|' was detected, after the '|' no more arguments but a NEW command follows */
			if (inputbuffer [i] == '|') {
	                        cmdbuffer [cmd].pipe = TRUE;
				while (inputbuffer [++i] == ' ' && inputbuffer [i] != '\0')
					;
				cmdindex = i;
				tmpbuf [0] = '\0';
				j = 0;
                        	break;
                	}

			/* redirection of output into file detected */
			if (inputbuffer [i] == '>') {
				while (inputbuffer [++i] == ' ' && inputbuffer [i] != '\0')
					;
				if (inputbuffer [i] == '\0')
					printf ("Error: Unexpected end of line\n");
				while (inputbuffer [i] != ' ' && inputbuffer [i] != '\0') {
					tmpbuf [j] = inputbuffer [i];
					i++;
					j++;
				}
				cmdindex = i;
				tmpbuf [j] = '\0';
				j = 0;

				cmdbuffer [cmd].outputfile = malloc (MAX_COMMANDLENGTH);
				strcpy (cmdbuffer [cmd].outputfile, tmpbuf);
#ifdef __DEBUG__
				printf ("DEBUG: Set cmdbuffer [%d].outputfile = %s\n", cmd, cmdbuffer [cmd].outputfile);
#endif

				tmpbuf [0] = '\0';
                        	break;
                	}

			/* redirection of input from file detected */
			if (inputbuffer [i] == '<') {
				while (inputbuffer [++i] == ' ' && inputbuffer [i] != '\0')
					;
				if (inputbuffer [i] == '\0')
					printf ("Error: Unexpected end of line\n");
				while (inputbuffer [i] != ' ' && inputbuffer [i] != '\0') {
					tmpbuf [j] = inputbuffer [i];
					i++;
					j++;
				}
				cmdindex = i;
				tmpbuf [j] = '\0';
				j = 0;

				cmdbuffer [cmd].inputfile = malloc (MAX_COMMANDLENGTH);
				strcpy (cmdbuffer [cmd].inputfile, tmpbuf);

#ifdef __DEBUG__
				printf ("DEBUG: Set cmdbuffer [%d].inputfile = %s\n", cmd, cmdbuffer [cmd].inputfile);
#endif

				tmpbuf [0] = '\0';
                        	break;
                	}

			/* if it was neither pipe or file redirection, then just pass the next string as argument */
			cmdbuffer [cmd].argn++;
			tmpbuf [j] = '\0';
			cmdbuffer [cmd].argv [cmdbuffer[cmd].argn - 1] = malloc (MAX_COMMANDLENGTH);
			strcpy (cmdbuffer [cmd].argv [cmdbuffer[cmd].argn - 1], tmpbuf);

			if (inputbuffer [i] == '\0')
				break;
	
		}

#ifdef __DEBUG__
	
		printf ("DEBUG: argn = %d\n", cmdbuffer [cmd].argn);
		printf ("DEBUG: cmdbuffer [%d].name = %s, pipe = %s\n", cmd, cmdbuffer [cmd].name, cmdbuffer [cmd].pipe ? "YES" : "NO");
		for (j = 0; j < cmdbuffer [cmd].argn; j++) {
			printf ("DEBUG: cmdbuffer [%d].argv [%d] = %s\n", cmd, j, cmdbuffer [cmd].argv [j]);
		}
#endif
		/* have we reached the end of the input, then STOP here */
		if (inputbuffer [i] == '\0')
			break;

	}



	(* cmdcount) = cmd + 1;
#ifdef __DEBUG__
	printf ("DEBUG: cmdcount: %d\n", *cmdcount);
#endif

}

/* freecmdbuffer - free command buffer allocated memory completely */

void freecmdbuffer (struct s_command cmdbuffer [MAX_COMMANDCOUNT], int cmdcount) {
	
	int i, j;

	for (i = 0; i < cmdcount; i++) {

#ifdef __DEBUG__
		printf ("DEBUG: Freeing cmdbuffer [%d].name\n", i);
#endif

		free (cmdbuffer [i].name);
		cmdbuffer [i].name = NULL;
#ifdef __DEBUG__
		printf ("DEBUG: Freeing cmdbuffer [%d].inputfile\n", i);
#endif
		free (cmdbuffer [i].inputfile);
		cmdbuffer [i].inputfile = NULL;

#ifdef __DEBUG__
		printf ("DEBUG: Freeing cmdbuffer [%d].outputfile\n", i);
#endif
		free (cmdbuffer [i].outputfile);
		cmdbuffer [i].outputfile = NULL;

		for (j = 0; j < cmdbuffer [i].argn; j++) {
#ifdef __DEBUG__
		printf ("DEBUG: Freeing cmdbuffer [%d].argv [%d]\n", i, j);
#endif
			free (cmdbuffer [i].argv [j]);
			cmdbuffer [i].argv [j] = NULL;
		}
	}
	
}

/* execcommand - execute command buffer */

void execcommand (struct s_command cmdbuffer [MAX_COMMANDCOUNT], int cmdcount) {

	int i;
//	int pids [MAX_COMMANDCOUNT];
//	int fds [MAX_COMMANDCOUNT];

	int pid1, pid2;
	int fds [MAX_COMMANDCOUNT - 1] [2]; /* to store the file-descriptors for the pipes, one pipe between each two commands */
	FILE * fp;

	/* shall command be piped ? */

	if (cmdbuffer [0].pipe) {

		/* open "cmdcount - 1" pipes (one pipe less than commands)
		 * for each successive pair of commands to pipe
		 */

		for (i = 0; i < cmdcount - 1; i++) {
			if (pipe (fds [i]) == -1) {
				printf("Error while calling pipe() !");
				return;
			}
#ifdef __DEBUG__
			printf ("DEBUG: Created %d-th pipe-pair: %d, %d\n", i, fds [i][0], fds [i][1]);
#endif
		}


		for (i = 0; i < cmdcount - 1; i++) {

			/* only fork here, if we are to exec the very first command which has NO pipe input */
			if (i == 0 && (pid1 = fork()) == 0) {

#ifdef __DEBUG__
				printf ("DEBUG: Pid1 = %d, fds [%d][%d] = %d, fds [%d][%d] = %d\n", pid1, i, 0, fds [i][0], i, 1, fds [i][1]);
#endif
				/* "redirect" STDOUT into pipe */
				dup2(fds [i][1], STDOUT_FILENO);
				
				if (i == 0)
					close(fds [i][0]); /* the first command does not have a pipe-input (i == 0) */
				else
					dup2(fds[i][0], STDIN_FILENO); /* for the remaining commands, their input
									    comes the previous command in the pipe-queue */
		
//				if (cmdbuffer [i].inputfile != NULL) {
//					if ((fp = freopen(cmdbuffer [i].inputfile, "r", stdin)) == NULL)
//					printf("Error while redirecting input-file !\n");
//				}
//

				/* redirect input/output into/from file */	
				if (cmdbuffer [i].inputfile != NULL) {
#ifdef __DEBUG__
					printf ("DEBUG: Will redirect input for cmdbuffer [%d] from file \"%s\"\n", i ,cmdbuffer [i].inputfile);
#endif
					if ((fp = freopen(cmdbuffer [i].inputfile, "r", stdin)) == NULL)
						printf("Error while redirecting input-file !\n");
				}

				if (cmdbuffer [i].outputfile != NULL) {
#ifdef __DEBUG__
					printf ("DEBUG: Will redirect output for cmdbuffer [%d] to file \"%s\"\n", i, cmdbuffer [i].outputfile);
#endif
					if ((fp = freopen(cmdbuffer [i].outputfile, "w", stdout)) == NULL)
						printf("Error while redirecting output-file !\n");
				}


				/* try to execute command */
				if (execvp(cmdbuffer [i].name, cmdbuffer [i].argv) == -1) {
					printf("Error while calling execvp() - cannot execute command !\n");
					return;
				}
			}
		
			/* fork for all subsequent command calls */
			else if ((pid2 = fork()) == 0) {

#ifdef __DEBUG__
				printf ("DEBUG: Pid2 = %d, fds [%d][%d] = %d, fds [%d][%d] = %d\n", pid2, i, 0, fds [i][0], i, 1, fds [i][1]);
#endif			
	
				dup2(fds [i][0], STDIN_FILENO); /* input from the current "outputting" command */
				if (cmdcount > 2 && i != cmdcount - 2) /* this "if" keeps us from redirecting output of the last command in the pipe queue */
					dup2(fds [i+1][1], STDOUT_FILENO); /* output to the next command, thus next pipe */
				close(fds[i][1]); /* fds [1] is for writing, we don't need it here */

//				if (cmdbuffer [i+1].outputfile != NULL) {
//					if ((fp = freopen(cmdbuffer [i+1].outputfile, "w", stdout)) == NULL)
//						printf("Error while redirecting output-file !\n");
//				}
				
				/* redirect input/output into/from file */
				if (cmdbuffer [i+1].inputfile != NULL) {
#ifdef __DEBUG__
					printf ("DEBUG: Will redirect input for cmdbuffer [%d] from file \"%s\"\n", i+1 ,cmdbuffer [i+1].inputfile);
#endif
					if ((fp = freopen(cmdbuffer [i+1].inputfile, "r", stdin)) == NULL)
						printf("Error while redirecting input-file !\n");
				}

				if (cmdbuffer [i+1].outputfile != NULL) {
#ifdef __DEBUG__
					printf ("DEBUG: Will redirect output for cmdbuffer [%d] to file \"%s\"\n", i+1, cmdbuffer [i+1].outputfile);
#endif
					if ((fp = freopen(cmdbuffer [i+1].outputfile, "w", stdout)) == NULL)
						printf("Error while redirecting output-file !\n");
				}
	
				if (execvp(cmdbuffer [i+1].name, cmdbuffer [i+1].argv) == -1) {
					printf("Error while calling execvp() - cannot execute command !\n");
					exit(1);
				}
			}

			
			/* close pipe's fds */
			close(fds[i][0]);
			close(fds[i][1]);
			
			/* shell must wait until child processes terminate, called for each fork() */
			wait (0);
			/* but only when executing the first command, we have to wait(0) twice, since we forked twice */
			if (i == 0)
				wait (0);	
		} 

	}

	/* no piping, just call program */

	else {
		if (fork () == 0) {

			if (cmdbuffer [0].inputfile != NULL) {
#ifdef __DEBUG__
				printf ("DEBUG: Will redirect input for cmdbuffer [0] from file \"%s\"\n", cmdbuffer [0].inputfile);
#endif
				if ((fp = freopen(cmdbuffer [0].inputfile, "r", stdin)) == NULL)
					printf("Error while redirecting input-file !\n");
			}

			if (cmdbuffer [0].outputfile != NULL) {
#ifdef __DEBUG__
				printf ("DEBUG: Will redirect output for cmdbuffer [0] to file \"%s\"\n", cmdbuffer [0].outputfile);
#endif
				if ((fp = freopen(cmdbuffer [0].outputfile, "w", stdout)) == NULL)
					printf("Error while redirecting output-file !\n");
			}
			if (execvp(cmdbuffer [0].name, cmdbuffer [0].argv) == -1) {
				printf("Error while calling execvp() - cannot execute command !\n");
				exit (-1);
			}
		}
		else {
			wait (0); /* wait until child-process from fork()-call finishes */
			return;
		}
	}
}

/* listhistory - lists contents of history-buffer */

void listhistory (char ** histbuffer, int histcount) {

	int i;

	printf ("History buffer:\n\n");

	for (i = 0; i < histcount; i++) {
	
		printf ("\tCommand, Nr. %d: %s\n", i + 1, histbuffer [i]);
	}

	printf ("\n");
}

/* puthistory - put new command line onto history buffer */

void puthistory (char * inputbuffer, int bufsize, char * histbuffer [], int * histcount) {

	int i;
	

	/* is history buffer full ? */
	if ((* histcount) == MAX_HISTCOUNT) {

		/* YES, it is, make space for the new entry */
		
		/* free buffer of oldest history entry */
		free (histbuffer [MAX_HISTCOUNT - 1]);
	}

	/* move all entries one slot higher, i.e. 5->6 */
	for (i = (* histcount) - 1; i >= 0; i--)
		histbuffer [i + 1] = histbuffer [i];


	/* malloc buffer for new history line, copy current commandline into it */
	histbuffer [0] = malloc (bufsize);
	memcpy (histbuffer [0], inputbuffer, bufsize);

	if ((* histcount) < MAX_HISTCOUNT)
		(* histcount)++;
#ifdef __DEBUG__
	printf ("DEBUG: histcount: %d\n", (*histcount));
#endif
}
