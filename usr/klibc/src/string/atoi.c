#include "ctype.h"

int	atoi(const char *nptr){
	int			sign;
	int			num;
	const char	*tmp;

	sign = 1;
	num = 0;
	tmp = nptr;
	while (isspace(*tmp))
		tmp++;
	if (*tmp == '-' || *tmp == '+'){
		if (*tmp == '-')
			sign *= -1;
		tmp++;
	}
	while (isdigit(*tmp))
		num = ((num << 3) + (num << 1)) + (*tmp++ - '0');
	return (num * sign);
}
