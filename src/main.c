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
	terminal_initialize(&terminals[0], 0);
	terminal_initialize(&terminals[1], 1);
	terminal_initialize(&terminals[2], 2);
	/*uint16_t cs;*/
	/*uint16_t ds;*/
	/*uint16_t ss;*/
	/**/
	/*printf("Before GDT FLUSH: \n");*/
	/*t_gdtr gdtr;*/
	/**/
	/*asm volatile ("sgdt %0" : "=m"(gdtr));*/
	/**/
	/*printf("sizeof = %x\n", sizeof(gdt_table));*/
	/*printf("limit  = %x\n", gdtr.limit);*/
	/**/
	gdt_init();
	/*printf("After GDT FLUSH: \n");*/
	/**/
	/*t_gdtr test;*/
	/**/
	/*asm volatile ("sgdt %0" : "=m"(test));*/
	/**/
	/*printf("CPU base  = %x\n", test.base);*/
	/*printf("CPU limit = %x\n", test.limit);*/

	printf(">");

	for (;;)
		// handle_keyboard_entry();
		return 1;
}
