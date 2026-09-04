#ifndef _KLIBC_STRING_H
# define _KLIBC_STRING_H

#include "unistd.h"

int		atoi(const char *nptr);
int		strcmp(const char *s1, const char *s2);
size_t	strlen(const char *s);
int		strncmp(const char *s1, const char *s2, size_t n);
void	*memmove(void *dest, const void *src, size_t n);

#endif
