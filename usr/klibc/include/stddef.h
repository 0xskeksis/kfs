#ifndef _KLIBC_STDDEF
# define _KLIBC_STDDEF

#include "stdint.h"

#define __SIZE_TYPE__ long unsigned int
#define __INTPTR_TYPE__ long int
#define __PTRDIFF_TYPE__ long int
#define NULL (void*)0

#endif
