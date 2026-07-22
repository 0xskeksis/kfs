

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

#include "vga.h"

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

static uint16_t terminal_buffers[TERMINAL_COUNT][VGA_SIZE];
static volatile uint16_t *const vga_memory = (volatile uint16_t *)VGA_MEMORY;

t_terminal terminals[TERMINAL_COUNT];
size_t current_term = 0;

void terminal_initialize(t_terminal *term, size_t id) 
{
	if (term == NULL || id >= TERMINAL_COUNT)
		return;

	term->row = 0;
	term->col = 0;
	term->id = id;
	term->color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

	term->buffer = terminal_buffers[id];
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) 
	{
		for (size_t x = 0; x < VGA_WIDTH; x++) 
		{
			const size_t index = y * VGA_WIDTH + x;
			term->buffer[index] = vga_entry(' ', term->color);
		}
		
		term->line_end[y] = 0;
	}
}

static void terminal_putentryat(t_terminal *term, char c, uint8_t color, size_t x, size_t y) 
{
	if (term == NULL)
		return;

	const size_t index = y * VGA_WIDTH + x;
	uint16_t entry = vga_entry(c, color);
	term->buffer[index] = entry;

	if (term->id == current_term)
		vga_memory[index] = entry;
}

static int handle_special_char(t_terminal *term, char c)
{
	if (term == NULL)
		return 1;

	switch (c)
	{
		case '\t':
			{
				size_t spaces = TAB_WIDTH - (term->col % TAB_WIDTH);
				while(spaces--)
					terminal_putchar(term, ' ');
				return 1;
			}
		case '\n': //Later handle command when \n
			{
				term->line_end[term->row] = term->col;

				term->col = 0;
				if (++term->row == VGA_HEIGHT)
					term->row = 0;
				
				term->line_end[term->row] = 0;
				return 1;
			}
		case '\b':
			{
				if (term->col == 0)
				{
					if (term->row == 0)
						return 1;
					
					term->row--;
					term->col = term->line_end[term->row];

					if (term->col == 0)
						return 1;
				}

				term->col--;

				terminal_putentryat(term, ' ', term->color, term->col, term->row);

				term->line_end[term->row] = term->col;
				return 1;
			}
	}

	return 0;
}

void terminal_putchar(t_terminal *term, char c) 
{
	if (term == NULL)
		return;

	if (handle_special_char(term, c))
		return;

	terminal_putentryat(term, c, term->color, term->col, term->row);
	if (++term->col == VGA_WIDTH) 
	{
		term->line_end[term->row] = VGA_WIDTH;
		term->col = 0;

		if (++term->row == VGA_HEIGHT)
			term->row = 0;

		term->line_end[term->row] = 0;
	}
}

void terminal_write(t_terminal *term, const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(term, data[i]);
}

void terminal_writestring(t_terminal *term, const char* data) 
{
	terminal_write(term, data, strlen(data));
}

void terminal_render(t_terminal *term)
{
	if (term == NULL)
		return;

	for (size_t i = 0; i < VGA_SIZE; i++)
		vga_memory[i] = term->buffer[i];
}
