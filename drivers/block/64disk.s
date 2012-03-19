;/*
;	64disk.s
;	c64 disk block i/o
;	pOS - portable Operating System
;	02/14/03
;	(c) A.Glaubitz
;*/

#include <driver.h>

;/*
;	- shall we support multiple disk drives ? (earlier or later anyway !)
;	(NOTE: the C64 can access only ONE disk-device at a time anyway)
;	- where to store the C64-SPECFIC floppy-address then ??
;*/

.export __64disk_init
.export __64disk_exit
.export __64disk_writeb
.export __64disk_readb

;/* status flag-byte:
;
;  0  0  0  0  0  0  0  0
;  X  X  X 11 10  9  8  RDY
;
;  X = reserved
;  11, 10, 9, 8 = current drive ID
;  RDY = ready, driver has initialized
;
;*/

__status:
.byte   0

; ----------------------------------------------

__64disk_init:

	lda __status
	ora #$01
	sta __status
	lda #$ff ; initialization successful (-1)
	pha ; if int is two bytes on 6502/-10
	pha ; else return only one byte
	rts

; ----------------------------------------------

__64disk_exit:
        
	lda __status
	and #$fe
	sta __status
	lda #$ff
	pha
	pha
	rts


; ----------------------------------------------

__64disk_writeb:

	lda #$00
	pha
	pha
	rts

; ----------------------------------------------

__64disk_readb:

	lda #$00
	pha
	pha
	rts
