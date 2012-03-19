/*
* pOS
* list.h
* 11/25/03
*/

#define m_list_pickup(list, counter, item) \
\
	list [counter] = item; \
	counter++;

#define m_list_kickout(list, counter, item) \
\
	*item = list [counter - 1]; \
	counter--;
