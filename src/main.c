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

#include "vga.h"
#include "stdio.h"
#include "keyboard.h"

int main(void) 
{
	terminal_initialize();

	//printf("%s %i %c %x %u", "hello", 12, 'c', 42, -12);
	for (;;)
		handle_keyboard_entry();
	return 1;
}
