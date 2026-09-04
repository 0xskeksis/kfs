#include "gdt.h"

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
	gdt_init();

	terminal_initialize(&terminals[0], 0);
	terminal_initialize(&terminals[1], 1);
	terminal_initialize(&terminals[2], 2);

	printf(">");

	for (;;)
		// handle_keyboard_entry();
		return 1;
}
