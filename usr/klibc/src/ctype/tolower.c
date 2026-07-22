#include "ctype.h"

int	ft_tolower(int c)
{
	if (isupper(c))
		return (c | 32);
	return (c);
}
