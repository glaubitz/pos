;/*
;	64serial.s
;	c64 serial driver
;	pOS - portable Operating System
;       03/14/03
;	(c) A.Glaubitz
;*/

#include <driver.h>

.export __64serial_init
.export __64serial_exit
.export __64serial_putc
.export __64serial_getc

;/* status flag-byte:
;
;  0  0  0  0  0  0  0  0
;  X  X  X  X  X  X  X RDY
;
;  X = reserved
;  RDY = ready, driver has initialized
;
;*/

__status:

.byte	$00

; ----------------------------------------------

__64serial_init:

	lda __status
	ora #$02 ; set driver status RDY-flag
	sta __status
	lda #$ff ; initialization successful (-1)
	pha ; if int is two bytes on 6502/-10
	pha ; else return only one byte
	rts

; ----------------------------------------------

__64serial_exit:

	lda __status
	and #$fd
	sta __status
	lda #$ff ; exit successful (-1)
	pha
	pha
	rts

; ----------------------------------------------

__64serial_putc:

	lda #$00
	pha
	pha
	rts

; ----------------------------------------------

__64serial_getc:

	lda #$00
	pha
	pha
	rts
