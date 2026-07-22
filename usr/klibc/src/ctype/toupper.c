#include "ctype.h"

int	toupper(char c)
{
	if (islower(c))
		return (c & 0x5f);
	return (c);
}
