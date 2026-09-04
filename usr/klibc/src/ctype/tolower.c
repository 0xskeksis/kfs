#include "ctype.h"

int	tolower(char c)
{
	if (isupper(c))
		return (c | 32);
	return (c);
}
