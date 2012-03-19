/*
* pOS
* timer.h
* 03/27/02
*/

typedef TIMERPROC void (*) (DWORD timervalue);

DWORD createtimer (TIMERPROC * timerproc, DWORD timerstart);
DWORD killtimer (TIMERPROC * timerproc);
