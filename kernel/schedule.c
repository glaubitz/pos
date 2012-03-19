/*
* pOS
* schedule.c - main scheduler code
* 07/18/05
*/

#define c_TICKSPERSECOND 64 // on c64
#define c_MAXNTASKS c_TICKSPERSECOND

// hmm, do we really want one 1 seconed for a round-trip ??

// IMPORTANT NOTICE: Since we are in interrupt we rather want to jmp
// to a call rather than jsr-ing into it, is there any C-keyword
// we can use to tell the compiler this ?

// ALTERNATIVELY we can do the call environment-saving directly
// in assembly at first in the C64 irq-handler

// 07/18/05 - only doubly linked lists need to be reconceived

void schedule () {

	// Tasks here:

	// 1. check for any signals for tasks and wake them up if needed
	//
	
	struct s_task * t;
	struct s_task * next;
	int n;
	t = current;
		
		
	while ((t = t->next) != current)
		if (t->state == c_STATEINTERRUPTIBLE && t->signal)
			t->state = c_STATERUNNING;

	// 2. check whether the number of running tasks has changed in the meanwhile
		n = 0; // starting at 0 to ignore idle task
		      // when distributing CPU-time
		(s_task *) t = current;
		while ((t = t->next) != current)
			if (t->state == c_STATERUNNING) n++;
	
		if (n != k_kernel.ntasks) {
			k_kernel.ticks_per_task = c_TICKSPERSECOND / k_kernel.ntasks;
			k_kernel.ticks_for_idle = c_TICKSPERSECOND % k_kernel.ntasks;
		}

	// 3. task switch

	// this is a point where we need doubly-linked lists ;)

	current = k_kernel.current->next;

	if (!current->pid == 0)
		current->jiffies = k_kernel.ticks_per_task;
	else
		current->jiffies = k_kernel.ticks_for_idle;

	// 4. if has signal, then call the signal handler

	if (current->signal)
		do_signal (current);

}

void do_signal (s_task * task) {
	
	if (current->pid == 0) {
		printk ("Kernel Warning: Idle process cannot receive signals, ignoring signal %d !", current->signal);
		return;
	}
	
	switch (current->signal) {

	case c_SIGTERM: c_SIGKILL: c_SIGUSR1: c_SIGUSR2: c_SIGUSR3:
		(void *) current->sigfnc [current->signal];
	
	default:
		printk ("Kernel Warning: Ignoring unexpected signal %d for process %d.", current->signal, current->pid);
	}
	current->signal = 0;
}

switch_to (s_task * task) {

	save_env (current);

	load_env (task);

	
}

k_idletask () {

	// well, nothing sensible to be done here ;)
	// more sophisticated machines call PM_routines here

	l1:
		goto l1;
}

k_timerirq () {

	k_kernel.ticks++;

	if (--current->jiffies)
		current->jiffies--;
	else
		schedule (); // oh oh, calling schedule will push even more data onto stack

}

asm {
__64_irq:

	m_saveenv
	; call driver interrupts here

	jmp _k_timerirq
	
	
	m_loadenv
	rti
}
