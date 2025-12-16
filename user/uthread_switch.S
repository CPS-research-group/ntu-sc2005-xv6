	.text

	/*
	* save the old thread's registers,
	* restore the new thread's registers.
	*/

	.globl thread_switch
thread_switch:
	/* TODO: save/restore regs */
	ret    /* return to ra */
