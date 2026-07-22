#ifndef _VGA
# define _VGA

#define VGA_COLOR_BLACK			0
#define VGA_COLOR_BLUE			1
#define VGA_COLOR_GREEN			2
#define VGA_COLOR_CYAN			3
#define VGA_COLOR_RED			4
#define VGA_COLOR_MAGENTA		5
#define VGA_COLOR_BROWN			6
#define VGA_COLOR_LIGHT_GREY	7
#define VGA_COLOR_DARK_GREY		8
#define VGA_COLOR_LIGHT_BLUE	9
#define VGA_COLOR_LIGHT_GREEN	10
#define VGA_COLOR_LIGHT_CYAN	11
#define VGA_COLOR_LIGHT_RED		12
#define VGA_COLOR_LIGHT_MAGENTA	13
#define VGA_COLOR_LIGHT_BROWN	14
#define VGA_COLOR_WHITE			15

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000
#define VGA_SIZE	(VGA_WIDTH * VGA_HEIGHT)

#define TAB_WIDTH 4

#define TERMINAL_COUNT 3

#include "stddef.h"
#include "stdint.h"
#include "unistd.h"

ssize_t	write(int fd, const void *buf, size_t count);

typedef struct
{
	size_t id;
	uint16_t *buffer;
	size_t col;
	size_t row;
	size_t line_end[VGA_HEIGHT];
	uint8_t color;
}	t_terminal;

extern t_terminal terminals[TERMINAL_COUNT];
extern size_t current_term;

void terminal_writestring(t_terminal *term, const char* data);
void terminal_write(t_terminal *term, const char* data, size_t size);
void terminal_putchar(t_terminal *term, char c);
void terminal_initialize(t_terminal *term, size_t id);
void terminal_render(t_terminal *term);

#define CURRENT_TERMINAL (&terminals[current_term])

#endif
