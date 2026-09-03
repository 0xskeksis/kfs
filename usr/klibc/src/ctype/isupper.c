#include "ctype.h"

int	isupper(char c)
{
	return ((unsigned)c - 'A' < 26);
}
