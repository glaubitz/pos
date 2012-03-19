;/*
;* pOS
;* entry.S - c64 entry routine
;* 01/14/03
;*/

#include "c64.h"

// define here that k_panic and k_start_kernel are EXTERN

; this here is the kernel entry code called by the bootloader

; ! THIS CODE MUST BE LINKED TO THE TOP OF THE KERNEL CODE !

; here the kernel parameters are determined:
;
; - memory size
; - non-usable memory blocks
;
; then the start_kernel funtion is called


        .org c_KRNLBASE

#include "jmptable.s"

entry_main:

	sei ; disable interrupts

	ldx #$ff
	txs ; reset stack

	; setup new interrupt/reset vectors

	ldx #<c64irq
	ldy #>c64irq
	stx $fffe
	sty $ffff

	ldx #<c64nmi
	ldy #>c64nmi
	stx $fffa
	sty $fffb

        ldx #<_k_panic
        ldy #>_k_panic
	stx $fffc
	sty $fffd

	; disable kernel & basic roms, i/o is enabled

	lda #63 ; first six bits set, all ports = outputs
	sta $00 ; don't they need to be inputs ?

	lda #20 ; first bits: BASICROM(ON|OFF), KERNALROM(ON|OFF),(I/O|CHARROM)
	sta $01 ; bit4 should be 1, bit 5 should be 0, 6 & 7 are unused

	; in case the main routine returns we know what happens

	lda #<FREEZE
	pha
	lda #>FREEZE
	pha

        jmp _k_start_kernel


FREEZE:

	jmp FREEZE

