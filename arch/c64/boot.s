;
; pOS Boot Loader - C64
; Version 0.0.6
; 05/30/05
;
; history:
;
; 0.0.1, 01/26/01: initial release, not runnable
; 0.0.2, 03/23/01: dozens of bugs fixed, should work now
; 0.0.3, 04/12/01: added boot message for kernel loading
; 0.0.4, 05/23/01: fixed EVERYTHING, finally works
; 0.0.5, 05/25/01: fixed wrong error message (disk error) when image not found
; 0.0.6, 05/26/05: little code cleanup

;#include "c64.h"
.define c_KRNLBASE $c000
.define c_IMAGEFILENAME "KERNEL"
.define c_BOOTDELAY 5 ; max 60 seconds

	.org $07ff

	.byte $01, $08 ; these two bytes will be cut by the LOAD routine
	
; <adress next line lo, hi>,<line number lo, hi>,"SYS",<Space>,"2","0","6","2",<end of line>, <adress next lo, hi>
	.byte $0c, $08, $01, $00, $9e, $20, $32, $30, $36, $32, $00, $00, $00

; loader entry point

	; set text color to grey
	
	lda #$0f
	sta $286

	; set background/frame color to black

	lda #$00
	sta $d020
	sta $d021

	; output loeader info (version)

	ldx #<loaderinfo
	ldy #>loaderinfo
	jsr stdout
	
	; FIXME/MISSING: set font to small
	
	; output bootmsg

	ldx #<bootmsg
	ldy #>bootmsg
	jsr stdout

	ldx #<imagefilename ; set kernel image file name
	ldy #>imagefilename
	; FIXME: should determine filename length dynamically (as fn is dynamic)
	lda #$06 ; this routine needs string length in accu
	jsr $ffbd ; SETNAM

	lda #$01 ; logical file number 1
	ldx #$08 ; drive 8
	ldy #$00 ; secondary adress 0
	jsr $ffba ; SETLFS
	
	lda #$00
;	sta $0a ; load flag, must be set ???
	ldx <c_KRNLBASE ; target adress lo-byte
	ldy >c_KRNLBASE ; target adress hi-byte
	jsr $ffd5 ; LOAD

	bcc kernel ; when carry clear->success
	
	lda #$00
	jsr $ffbd ; SETNAM & SETLFS MUST ALWAYS BE CALLED BEFORE OPEN
	
	lda #$0f ; LFN 15
	ldx #$08 ; drive 8
	ldy #$0f ; SECA 15
	jsr $ffba ; SETLFS
	
	jsr $ffc0 ; OPEN

	ldx #$0f ; LFN 15
	jsr $ffc6 ; input from logical file 15

	jsr $ffcf ; get first character
	sta cbmerror ; as ASCII Code
	jsr $ffcf
	sta cbmerror+1

N:	jsr $ffcf ; get next character
	cmp #$0d ; is it <RETURN> ?
	bne N ; no, then fetch next

	jsr $ffcc ; change to std. output

	lda #$0f ; LFN 15
	jsr $ffc3 ; close floppy channel 15

	ldx #<msgfail
	ldy #>msgfail
	jsr stdout
	
	ldx #<cbmerror
	ldy #>cbmerror
	jsr stdout
	rts ; leave to basic

kernel:
	
	ldx #<msgok
	ldy #>msgok
	jsr stdout

	jmp c_KRNLBASE

stdout:

	stx $fb
	sty $fc
	ldy #$00
B:	lda ($fb), y ; lda (op),y works with zp-ops only
	cmp #$00
	beq F
	jsr $ffd2
	iny
	cpy #$ff
	bne B
F:	rts

seconds:
	.byte $00

cbmerror:
	.byte $00, $00, $0d, $00

loaderinfo:
	.byte	$0d, "pOS boot loader for c64, 0.0.6", $0d
	.byte	$00

bootmsg:
	.byte	$0d, "about to boot..."
	.byte	$00

msgok:
	.byte	$0d, "success, now starting kernel"
	.byte	$0d, $00

msgfail:
	.byte	$0d, "failed, cbm-dos error "
	.byte	$00

imagefilename:
	.byte	c_IMAGEFILENAME
