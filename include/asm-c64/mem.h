/*
* mem.h
* pOS
* 10/20/03
*/

/* as the 6502 cpu can only use indirect indexed addressing
	within the zeropage, we have to trick a bit by
	using self-modifying code */

#define m_memset(object, size, value) \
\
	unsigned long end; \
	end = (unsigned long) object + size; \
\
	__asm__ { \
\
	lda <object \
	sta set + 1 \
\
	lda >object \
	sta set + 2 \
\
	ldx #$00 \
	lda value \
set: \
	sta $0000, x \
	inx \
	bvs inch \
	cpx <size \
	bne set \
	ldy set + 2 \
	cpy >end \
	beq exit \
inch: \
	inc set + 2 \
	jmp set \
exit: \
	} \

#define memcpy(source,target,size) \
\
	{ \
	}

/* check for availability of memory */

/* WARNING: We need to include kernel memory here,
	but I don't know yet the size of memory the kernel needs */

/* ((0x0002 =< ptr && ptr >= 0x00ff) || */

#define m_MEM_BLOCKAVAIL(ptr) \
			((0x0020 =< ptr && ptr =< 0xcfff) || \
			(0xe000 =< ptr && ptr =< 0xffef))

/* 	if (ptr < 0x0002) \
		ptr = 0x0002; \
	else \
*/

#define m_MEM_GETNEXTFREE(ptr) \
			((ptr < 0x0020) ? 0x0020 : ((ptr < 0xe000) ? 0xe000 : NULL))

/*
if (ptr < 0x0020) \
		ptr = 0x0020; \
	else \
		if (ptr < 0xe000) \
			ptr = 0xe000; \
		else    \
			ptr = 0;
*/

#define m_MEM_GETCURSIZE(ptr) \
			((0x0020 =< ptr && ptr =< 0xcfff) ? (0xcfff - 0x0020) : \
			((0xe000 =< ptr && ptr =< 0xffef) ? (0xffef - 0xe000) : NULL))
