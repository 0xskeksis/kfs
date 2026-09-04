#include "ctype.h"

int	isspace(char c)
{
	return (c == ' ' || (unsigned)c - '\t' < 5);
}
