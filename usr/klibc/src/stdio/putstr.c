#include "unistd.h"
#include "string.h"

int	putstr(char	*str){
	return write(1, str, strlen(str));
}
