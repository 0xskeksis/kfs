#include "ctype.h"

int	isdigit(char c)
{
	return ((unsigned int)c - '0' < 10);
}
