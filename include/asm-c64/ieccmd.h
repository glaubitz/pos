/*
* ieccmd.h
* Assembly Macros for controlling C64s IEC-bus
* 05/15/05
*/

#define CIA1BASE $dc00
#define	CIA2BASE $dd00

/* pulls data line high */

#define m_DATAhi \
	\
	__asm__ { \
	\
	lda  #%00100000 \
	ora  CIA2BASE \
	sta  CIA2BASE \
	}

#define m_DATAlow \
	\
	__asm__ { \
	\
	lda  #%11011111 \
	and  CIA2BASE \
	sta  CIA2BASE \
	}

#define m_CLOCKhi \
	\
	__asm__ { \
	\
	lda  #%00010000 \
	ora  CIA2BASE \
	sta  CIA2BASE \
	}

#define m_CLOCKlow \
	\
	__asm__ { \
	\
	lda  #%11101111 \
	and  CIA2BASE \
	sta  CIA2BASE \
	}

#define m_ATNhi \
	\
	__asm__ { \
	\
	lda  #%00001000 \
	ora  CIA2BASE \
	sta  CIA2BASE \
	}

#define m_ATNlow \
	\
	__asm__ { \
	\
	lda  #%11110111 \
	and  CIA2BASE \
	sta  CIA2BASE \
	}	
