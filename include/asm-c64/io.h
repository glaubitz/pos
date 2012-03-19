/*
* include/asm-c64/io.h
* pOS
* 08/30/02
*/

// ain't this stuff here superflous anyway ?

#define m_writeb(addr, b) (* (void *) addr) = b
#define m_readb(addr) (* (void *) addr)

#define m_writew(addr, w) (* (void *) addr) = w
#define m_readw(addr) (* (void *) addr)
