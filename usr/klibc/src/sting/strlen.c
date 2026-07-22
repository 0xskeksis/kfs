#include "unistd.h"
#include <stddef.h>

size_t	strlen(const char *s){
	char	*start = s;

	while (*s){
		s++;
	}
	return s - start;
}
