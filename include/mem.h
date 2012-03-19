/*
* mem.h
* pOS - portable Operating System
* 10/31/02
*/

#include <types.h>

#define m_GET_NEXT_MEMBLOCK(thismemblock) \
        ((MEMBINFO *) (thismemblock + thismemblock->size \
        + sizeof (MEMBINFO)))

#define m_GET_PREV_MEMBLOCK(thismemblock) \
        NULL
