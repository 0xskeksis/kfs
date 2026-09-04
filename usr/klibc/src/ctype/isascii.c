#include "ctype.h"

int	isascii(char c)
{
	return (!(c & ~127));
}
