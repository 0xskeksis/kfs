#include "keyboard.h"
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

/*
 * Code de test trouve sur le wiki, je referais un driver VGA
 *
 *
 *
 *
 *
 *
 *
 */

#include "stdint.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

typedef unsigned int size_t;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
size_t terminal_line_end[VGA_HEIGHT];

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) 
	{
		for (size_t x = 0; x < VGA_WIDTH; x++) 
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
		terminal_line_end[y] = 0;
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

#define TAB_WIDTH 4
void terminal_putchar(char c); 

int handle_special_char(char c)
{
	switch (c)
	{
		case '\t':
			{
				size_t spaces = TAB_WIDTH - (terminal_column % TAB_WIDTH);
				while(spaces--)
					terminal_putchar(' ');
				return 1;
			}
		case '\n': //Later handle command when \n
			{
				terminal_line_end[terminal_row] = terminal_column;

				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT)
					terminal_row = 0;
				
				terminal_line_end[terminal_row] = 0;
				return 1;
			}
		case '\b':
			{
				if (terminal_column == 0)
				{
					if (terminal_row == 0)
						return 1;
					
					terminal_row--;
					terminal_column = terminal_line_end[terminal_row];

					if (terminal_column == 0)
						return 1;
				}

				terminal_column--;

				terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);

				terminal_line_end[terminal_row] = terminal_column;
				return 1;
			}
	}

	return 0;
}

void terminal_putchar(char c) 
{
	if (handle_special_char(c))
		return;

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) 
	{
		terminal_line_end[terminal_row] = VGA_WIDTH;
		terminal_column = 0;

		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;

		terminal_line_end[terminal_row] = 0;
	}
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}
#include "kernel/vga.h"

#include "ctype.h"
#include "stdio.h"

int main(void) 
{
	terminal_initialize();

	for (;;)
	{
		unsigned char scancode;

		int res = keyboard_read_scancode(&scancode);
		if (res == 0)
			continue;

		key_event event;
		res = keyboard_decode_byte(scancode, &event);

		if (res == 0)
			continue;

		keyboard_update_state(event.key, event.pressed);
		if (!event.pressed)
			continue;

		char character = keycode_to_char(event.key);
		if (character == '\0')
			continue;

		terminal_putchar(character);
	}
	printf("%s %i %c %x %u", "hello", 12, 'c', 42, -12);
	return 1;
}
