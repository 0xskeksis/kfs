
__attribute__((noreturn))
void
__exit(int status){
	asm volatile(
		"int $0x80"
		:
		: "a"(1),
		"b"(status)
		:"ecx" , "memory"
	);
	__builtin_unreachable();
}

#include "kernel/vga.h"

int main(void) 
{
	terminal_initialize();

	terminal_writestring("Hello, kernel World!\n");
	return 1;
}
