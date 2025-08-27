#ifndef __STDDEF_H__
#define __STDDEF_H__ 0x1

#define NULL ((void *)(0x00))

#ifdef __SIZE_TYPE__
#undef __SIZE_TYPE__
#endif /* __SIZE_TYPE__ */

#define __SIZE_TYPE__ long unsigned int
typedef __SIZE_TYPE__ size_t;

#endif /* __STDDEF_H__ */