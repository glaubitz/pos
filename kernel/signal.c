/*
 * pOS
 * signal.c
 * 01/22/03
 */


#include <kernel.h>


void k_siguser () {

        k_setlasterror (c_MSG_UNKNOWN_SIGNAL);
}

int k_sendsignal (TASKINFO * process, BYTE signal) {

        k_setlasterror (c_MSG_ILLEGAL_SIGNAL);

	return FALSE;
}

void k_sigkill () {

	/* kill this task */

}

void k_sigsleep () {


	/* temporarily disable this tasks execution */
}

void k_sigwakeup () {

	/* undo signal sleep */

}
