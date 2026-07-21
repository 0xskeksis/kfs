#include "ctype.h"

int isascii(char c){
	return !(c&~0x7f);
}
