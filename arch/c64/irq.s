;/*
;* pOS
;* irq.s - c64 irq handler
;* 08/30/02
;*/

.export c64irq

// define here that _k_schedule is EXTERN (see also entry.s)
// (see assembler manual for syntax !)

c64irq:

;; MISSING: check for break-flag to tell a sw-irq from a hw-irq

;; put registers onto stack

;; 08/26/03: hmm, doesn't the 6502 push the regs onto stacks
;; automatically on interrupt ???
;; also we need to save the status-register too

	pha
	txa
	pha
	tya
	pha

        jmp _k_schedule ; jmp, because we must NOT alter the stack content;
			; the rts (or ret) is done in __schedule ()
