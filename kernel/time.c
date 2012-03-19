/*
 * pOS
 * time.c - time functions
 * 01/15/05
 */

/* TODO:
 * - write ticks_to_msecs / msecs_to_ticks / gettickcount / ... (either here
 *   or in schedule.c)
 * - BUG: msecs_to_time and time_to_msecs do not handle the time-offset
 *   1.1.2000 properly, need to debugged
 */


#include <kernel.h>


struct s_time * k_time_get (struct s_time * time) {

	unsigned long msecs;

	msecs = k_ticks_to_msecs (kernel->tickcount);

	msecs_to_time (msecs, time);

	/* basedate: 1.1.2000 */
	
	time->years += 2000;
	time->months += 1;

	return time;
}

int k_time_set (struct s_time * newtime) {
	
	struct s_time time;
	unsigned long msecs_up, msecs_new;

	msecs_up = k_ticks_to_msecs (kernel->tickcount);
	time = *newtime;
	time.years -= 2000;
	time.months -= 1;
	msecs_new = time_to_msecs (&time);

	if (msecs_new <= msecs_up)
		return FALSE;
	
	msecs_new = msecs_new - msecs_up;
	msecs_to_time (msecs_new, &time);

	time.years += 2000;
	time.months += 1;
	
	* kernel->boottime = time;

	return TRUE;
}

static unsigned char days_per_month [] = {

	31, 28, 31, 30,
	31, 30, 31, 31,
	30, 31, 30, 31
};

/* both "msecs_to_time" and "time_to_msecs" use relative times,
 * e.g: 0.0.0000 = 1.1.2000 (as 1.1.2000 is the systems base-date)
 */

static void msecs_to_time (unsigned long msecs, struct s_time * time) {

	unsigned long s, d, e, i;

	s = msecs / 1000;

	i = 3;
	
	while (1) {

		if (!(i % 3)) {
			if (s < 31622400)
				break;
			else {
				s -= 31622400;
				time->years++;
			}
		}
		else {
			if (s < 31536000)
				break;
			else {
				s -= 31536000;
				time->years++;
			}
		}

		if (!i--)
			i = 3;
	}

	d = s / 86400;
	s = s % 86400;

	time->hours = s / 3600;
	s = s % 3600;

	time->minutes = s / 60;
	s = s % 60;

	time->seconds = s;

	i = 1;
	e = 31;

	while (d > e) {

		e += days_per_month [i];

		if (!(time->years % 4) && (i == 1))
			e++;

		i++;
	}
	
	time->months = i - 1;
	time->days = d - (e - days_per_month [i - 1]);
}

static unsigned long time_to_msecs (struct s_time * time) {

	unsigned long msecs;
	unsigned char i;
	
	if (time->years < 2000 ||
	    time->months < 1 || time->months > 12 ||
	    time->days < 1 || time->days > 31 ||
	    time->hours > 23 ||
	    time->minutes > 59 ||
	    time->seconds > 59)
		return FALSE;

	msecs += time->years * 31536000;
	msecs += ((time->years / 4) + 1) * 86400;

	for (i = 0; i < 12; i++) {

		msecs += days_per_month [i] * 86400;
		if (!(time->years % 4) && i == 1)
			msecs += 86400;
	}

	msecs += time->days * 86400;
	msecs += time->hours * 3600;
	msecs += time->minutes * 60;
	msecs += time->seconds;

	msecs *= 1000;

	return msecs;
}
