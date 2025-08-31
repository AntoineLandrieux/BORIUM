#ifndef __STDARG_H__
#define __STDARG_H__ 0x1

/* #pragma once */

// Variable argument list
typedef char *va_list;

// Variable argument list initialization
#define va_start(va, arg) (va = (va_list)&arg + sizeof(arg))

// Get the next argument from the variable argument list
#define va_arg(va, type) (*(type *)((va += sizeof(type)) - sizeof(type)))

// Clean up the variable argument list
#define va_end(va) (va = NULL)

#endif /* __STDARG_H__ */