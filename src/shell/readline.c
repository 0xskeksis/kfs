#include <stdio.h>
#include <vga.h>
#include <string.h>

void getline(t_terminal *term, unsigned int row, char dest[VGA_WIDTH])
{
	if (row >= VGA_HEIGHT)
	{
		dest[0] = '\0';
		return;
	}

	size_t line_size = term->line_end[row];

	if (line_size >= VGA_WIDTH)
		line_size = VGA_WIDTH - 1;

	for (size_t i = 0; i < line_size; i++)
		dest[i] = (char)(term->buffer[row * VGA_WIDTH + i] & 0xFF);

	dest[line_size] = '\0';
}

void readline(t_terminal *term, char line[VGA_WIDTH])
{
	getline(term, term->row, line);
}
