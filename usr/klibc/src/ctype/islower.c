#include "ctype.h"

int	islower(char c)
{
	return ((unsigned int)c - 'a' < 26);
}
