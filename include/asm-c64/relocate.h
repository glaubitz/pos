/*
* relocate.h
* 08/30/02
* pOS
*/

#define m_relocate(offset, length) \
\
	__asm__ { \
\
		jmp relocate \
\
		reloc_base: \
			.word	00 \
\
		reloc_offset: \
			.word	00 \
	\
		relocate: \
\
		rts \
\
		; determine whether current jmp/jsr-instruction needs relocation \
\
		needs_reloc: \
\
		clc \
		rts \
\
		ins_length: \
			.byte	00 \
			.byte	00 \
	} \
