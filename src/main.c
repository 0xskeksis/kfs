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
#include "io/keyboard.h"
#include "shell.h"

int shell_mode = 1;

int main(void) 
{
	terminal_initialize(&terminals[0], 0);
	terminal_initialize(&terminals[1], 1);
	terminal_initialize(&terminals[2], 2);
	printf(">");

	//printf("%s %i %c %x %u", "hello", 12, 'c', 42, -12);
	for (;;)
		handle_keyboard_entry();
	return 1;
}
