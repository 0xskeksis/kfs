#include "ctype.h"

int isalpha(char c){
	return ((unsigned)c|32)-'a' < 26;
}
