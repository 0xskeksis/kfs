#include "ctype.h"

int	isprint(char c)
{
	return ((unsigned int)c - 32 < 95);
}
