#include "unistd.h"

int	putchar(char c){
	return write(1, &c, 1);
}
