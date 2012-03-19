/*
* pOS
* schedule.h
* 10/27/05
* arch specific scheduler code
*/

/* TODO:
 *
 * - how to init with an empty array ?
 * - all the operands for the register-saves/restores (__kernel. ...)
 *   certainly won't work in assembly :(  (test that for cc65 !)
 */

typedef struct __TASKENV {

	unsigned char stack [__MAX_STACKSIZE];
	unsigned char reg_accu;
	unsigned char reg_x;
	unsigned char reg_y;
	unsigned char reg_ps;
	unsigned char reg_sp;
	unsigned char pc_lo;
	unsigned char pc_hi;

} TASKENV;

#define c_inittaskenv \
\
{, \
	{}, \
	0, \
	0, \
	0, \
	32, \
	255, \
	(unsigned char) heap, \
	(unsigned char) heap << 8 \
} \

#define m_saveenv \
\
	__asm__ { \
\
		pla \
                sta __kernel.current->taskenv.reg_y \
		pla \
                sta __kernel.current->taskenv.reg_x \
                pla \
                sta __kernel.current->taskenv.reg_accu \
		pla \
                sta __kernel.current->taskenv.reg_ps \
                pla \
                sta __kernel.current->taskenv.pc_lo \
                pla \
                sta __kernel.current->taskenv.pc_hi \
                tsx \
		stx __kernel.current->taskenv.sp \
save_stack_loop: \
                pla \
                sta __kernel.current->taskenv.stack, x \
                dex \
                bpl save_stack_loop \
	} \

#define m_loadenv \
\
	__asm__ { \
\
                ldx #$00 \
load_stack_loop: \
                lda __kernel.current->taskenv.stack, x \
                pha \
                inx \
                cpx __kernel.current->taskenv.sp \
                bne load_stack_loop \
\
                lda __kernel.current->taskenv.pc_hi \
                pha \
                lda __kernel.current->taskenv.pc_lo \
                pha \
                lda __kernel.current->taskenv.reg_ps \
                pha \
                ldy __kernel.current->taskenv.reg_y \
                ldx __kernel.current->taskenv.reg_x \
                lda __kernel.current->taskenv.reg_accu \
	} \

// Well, them two are not pretty sophisticated:

#define m_lock __asm__ sei

#define m_unlock __asm__ cli
