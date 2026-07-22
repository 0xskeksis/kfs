

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

#include "kernel/vga.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

static inline uint8_t vga_entry_color(uint16_t fg, uint16_t bg) 
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


static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
static size_t terminal_line_end[VGA_HEIGHT];

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	
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

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

static int handle_special_char(char c)
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
