;/*
;	64con.s
;	c64 console driver
;	pOS - portable Operating System
;       06/15/05
;	(c) A.Glaubitz
;*/

.include <driver.h>

.define __VICBASE $d400
.define __SCRBUF  $0400
.define __NCOLS   40
.define __NROWS   25
.define __TXTCLR  14
.define __SPCCLR  7

;/* ESCAPE CODES */

.define __CHRESCAPE $00
.define __ESCDELETE $00
.define __ESCCSRUP  $00
.define __ESCCSRDWN $00
.define __ESCCSRRGT $00
.define __ESCCSRLFT $00
.define __ESCRETURN $0d
.define __ESCBLANK  $00

;/*
;TODO:
;
;- implement 64con_irq which called by irq.s during timer-
;  interrupt and polls the keyboard and wakes up assiocated
;  task once the there's been input
;- implement 64con_getc which should call schedule.c/suspend
;- 64con_getc should always echo any character typed (option)
;
;BUGS:
;
;- sta(PTR) instruction does NOT exist, sta(PTR),y is ZP only
;        --> use self-modifying code in such cases, this is faster
;            anyway :)
;- avoid compliacted calculating of screen address
;        --> yep, done
;- to increase speed we should NOT re-calculate screen memory
;  address of current position each time putc is called but
;  only do that when the cursor is moved
;- also consider using zeropage-pointers to copy large strings
;  onto screen
;- watch for special chars like '\r' (we're now using escape-codes)
;- disable taskswitching when using GLOBAL variables
;- characters are not converted from ASCII to screencode yet
;- replace underline character with reverse character from
;  standard c64 mode but do not use cursor blinking
;
;NOTES:
;
;- the color ram from $d800 stores the color value of
;  each character displayed, so we should consider
;  resetting it, too, when clearing the screen
;- we should introduce UNIX escape codes to move
;  the cursor, clear the screen and other stuff
;  (work in progress)
;- we could use two 25-byte tables with the lo and
;  hi-byte values of the addresses of the first
;  column of the screen in ram, this way we could
;  simple load the address instead of calculating
;  it
;*/

.export __64con_init
.export __64con_exit
.export __64con_putc
.export __64con_getc

;/* current char:
;
;  stores the current character "under"
;  the cursor; the cursor overwrites
;  the character at its position, when
;  the cursor moves the original character
;  is restored
;
;*/

__current_char:

.byte	$00

;/* current cursor position:
;
;   lo-byte = x-position (column)
;   hi-byte = y-position (row)
;
;*/

__csrpos:

.word	$00

__tmpword:

.word	$00

;/* status flag-byte:
;
;  0  0  0  0  0  0  0  0
;  X  X  X  X  X ECH RDY ESC
;
;  X = reserved
;  ECH = echo mode
;  RDY = ready, driver has initialized
;  ESC = escape mode
;
;*/

__status:

.byte	$00

; ----------------------------------------------

__64con_init:

	sei

        lda #>__SCRBUF 
        lsr
	lsr
	sta <__tmpword

	lda __VICBASE + $18
	and #$0f
	ora $<__tmpword
	
	sta __VICBASE + $18
	cli

; what must be done further ?

	lda __status
	ora #$02 ; set driver status RDY-flag
	sta __status
	lda #$ff ; initialization successful (-1)
	pha ; if int is two bytes on 6502/-10
	pha ; else return only one byte
	rts

; ----------------------------------------------

__64con_exit:

	; do further exit code here, if needed

	lda __status
	and #$fd
	sta __status
	lda #$ff
	pha
	pha
	rts

; ----------------------------------------------

__64con_putc:

	lda __status
	and #$02
	cmp #$02 ; RDY-flag set ??
	beq __putc_ready ; yes, go on
	sec ; NO, return with error
	rts

__putc_ready:

	lda __status
	and #$01
	cmp #$01 ; check for ESC-flag set
	bne __putc_noesc ; not set

	jmp __putc_esc

__putc_noesc:

	pla ; fetch character
	cmp __CHRESCAPE ; is it escape code
	bne __putc_main ; NO
	lda __status
	ora #$01 ; yes, set ESC-flag
	sta __status
	clc
	rts

__putc_main:

        jsr __asc2poke
	jsr __print_char

	jsr __esc_csrright

	clc
	rts

__putc_esc:

	pla ; fetch character

	cmp #__ESCDELETE
	beq __esc_delete
	cmp #__ESCCSRUP
	beq __esc_escup
	cmp #__ESCCSRDWN
	beq __esc_escdwn
	cmp #__ESCCSRRGT
	beq __esc_right
	cmp #__ESCCSRLFT
	beq __esc_left
	cmp #__ESCRETURN
	beq __esc_return
	cmp #__ESCBLANK
	beq __esc_blank

	; no valid escape-character found, disable escape mode, return error

	sec

__esc_done:

	lda __status
	and #$fe
	sta __status
	rts

; ------------- escape code routines -----------

__esc_delete:

	lda #$20 ; <space>

	jsr __print_char
	clc
	jmp __esc_done

__esc_csrup:

	lda#__current_char
	jsr __print_char ; overwrite cursor

	ldy >__csrpos
	cpy #$00

	beq __esc_csrup_isfirstrow
	dey
	sty >__csrpos

__esc_csrup_isfirstrow:

	jsr __show_csr
	clc
	jmp __esc_done

__esc_csrdwn:

	lda#__current_char
	jsr __print_char ; overwrite cursor

	ldy >__csrpos
	cpy #__NROWS
	beq __esc_csrdwn_islastrow
	iny
	sty >__csrpos
	
	jsr __show_csr
	clc
	jmp __esc_done

__esc_csrdwn_islastrow:

	jsr __scroll_up

	jsr __show_csr
	clc
	jmp __esc_done

__esc_csrleft:

	lda#__current_char
	jsr __print_char ; overwrite cursor

	clc
	jmp __esc_done

__esc_csrright:

	lda#__current_char
	jsr __print_char ; overwrite cursor

	clc
	jmp __esc_done

__esc_return:

	lda #__current_char
	jsr __print_char

	ldx #$00
	stx <__csrpos

	jmp __esc_csrdwn


__esc_blank:

	ldx #$00
	lda #$20

__esc_blank_l1:

	sta __SCRBUF,x
	sta __SCRBUF + $100,x
	sta __SCRBUF + $200,x
	cpx #$e8
	beq __esc_blank_skip1 ; we have to skip the last 23 bytes of this page
	sta __SCRBUF + $300,x ; since the screen buffer is 1000 bytes long not 1024

__esc_blank_skip1:
	
	inx
	bne __esc_blank_l1

	ldx #$00
	lda #__SPCCLR

__esc_blank_l2:

	sta $d800,x
	sta $d800 + $100,x
	sta $d800 + $200,x
	beq __esc_blank_skip2 ; same as for the screen buffer
	sta $d800 + $300,x
	
__esc_blank_skip2:

	inx
	bne __esc_blank_l2	

	clc
	jmp __esc_done

; ------------- screen routines ----------------

__print_char:

	pha ; temporarily store char on stack

; determine screen address from cursor position

	clc ; clear carry to determine overflow in the following addition
	ldy >__csrpos
	lda <__csrpos
	adc __scr_coladr_lobyte,y
	sta <__print_char_sm1 + 1
	sta <__print_char_sm2 + 1

	lda __scr_coladr_hibyte,y
	adc #>__SCRBUF
	sta >__print_char_sm1 + 1

	lda __scr_coladr_hibyte,y
	adc #$d8 ;  ???, hibyte of color ram base
	sta >__print_char_sm2 + 1
 
	pla ; now fetch the character back from stack

__print_char_sm1:

	sta $0000

	lda #__TXTCLR

__print_char_sm2:

	sta $0000

	rts

__show_csr:

	;; read char at current position

	sta __current_char
	lda #$64 ; = asc ('_')
	jsr __print_char ; hey, we must not simply print a '_' but keep the underlying char

	rts

__scroll_up:

	ldx #$00

__scroll_up_l1:

        lda __SCRBUF + $28,x
        sta __SCRBUF + $00,x
        lda __SCRBUF + $50,x
        sta __SCRBUF + $28,x
        lda __SCRBUF + $78,x
        sta __SCRBUF + $50,x
        lda __SCRBUF + $a0,x
        sta __SCRBUF + $78,x
        lda __SCRBUF + $c8,x
        sta __SCRBUF + $a0,x

        lda __SCRBUF + $f0,x
        sta __SCRBUF + $c8,x
        lda __SCRBUF + $118,x
        sta __SCRBUF + $f0,x
        lda __SCRBUF + $140,x
        sta __SCRBUF + $118,x
        lda __SCRBUF + $168,x
        sta __SCRBUF + $140,x
        lda __SCRBUF + $190,x
        sta __SCRBUF + $168,x

        lda __SCRBUF + $1b8,x
        sta __SCRBUF + $190,x
        lda __SCRBUF + $1e0,x
        sta __SCRBUF + $1b8,x
        lda __SCRBUF + $208,x
        sta __SCRBUF + $1b8,x
        lda __SCRBUF + $230,x
        sta __SCRBUF + $208,x
        lda __SCRBUF + $258,x
        sta __SCRBUF + $230,x

        lda __SCRBUF + $280,x
        sta __SCRBUF + $258,x
        lda __SCRBUF + $2a8,x
        sta __SCRBUF + $280,x
        lda __SCRBUF + $2d0,x
        sta __SCRBUF + $2a8,x
        lda __SCRBUF + $2f8,x
        sta __SCRBUF + $2d0,x
        lda __SCRBUF + $320,x
        sta __SCRBUF + $2f8,x

        lda __SCRBUF + $348,x
        sta __SCRBUF + $320,x
        lda __SCRBUF + $370,x
        sta __SCRBUF + $348,x
        lda __SCRBUF + $398,x
        sta __SCRBUF + $370,x
        lda __SCRBUF + $3c0,x
        sta __SCRBUF + $398,x

	lda #$20 ; empty character
        sta __SCRBUF + $3c0,x
	
	cpx #$28
	bne __scroll_up_l1

	rts

__scr_coladr_lobyte:

.byte <$00, <$28, <$50, <$78, <$a0
.byte <$c8, <$f0, <$118, <$140, <$168
.byte <$190, <$1b8, <$1e0, <$208, <$230
.byte <$258, <$280, <$2a8, <$2d0, <$2f8
.byte <$320, <$348, <$370, <$398, <$3c0

__scr_coladr_hibyte:

.byte >$00, >$28, >$50, >$78, >$a0
.byte >$c8, >$f0, >$118, >$140, >$168,
.byte >$190, >$1b8, >$1e0, >$208, >$230
.byte >$258, >$280, >$2a8, >$2d0, >$2f8
.byte >$320, >$348, >$370, >$398, >$3c0




; ----------------------------------------------

__64con_getc:

;; see "Commodore 64 - Die besten Tips und Tricks", page 91ff

; reads status from keyboard ports A and B
; returns ASCII character-code or ESC code sequence (i.e. CSR-LEFT)
; must be somehow called during interrupt as the keyboard
; does NOT generate any signal, so we have to check it periodically

	lda __status
	and #$02
	cmp #$02 ; RDY-flag set ??
	beq __getc_ready ; yes, go on
	sec ; NO, return with error
	rts

__getc_ready:

	clc
	rts
