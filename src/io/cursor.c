#include <vga.h>
#include <io/io.h>
#include <stddef.h>
#include <stdint.h>

#define CURSOR_REGISTER_SELECTION_PORT	0x3D4
#define CURSOR_REGISTER_DATA_PORT		0x3D5

#define CURSOR_LOW_BYTE_REGISTER		0x0F
#define CURSOR_HIGH_BYTE_REGISTER		0x0E

size_t cursor_pos(size_t row, size_t col)
{
	return row * VGA_WIDTH + col;
}

void set_cursor_pos(size_t pos)
{
	outb(CURSOR_REGISTER_SELECTION_PORT, CURSOR_LOW_BYTE_REGISTER);
	outb(CURSOR_REGISTER_DATA_PORT, (uint16_t)pos & 0xFF);

	outb(CURSOR_REGISTER_SELECTION_PORT, CURSOR_HIGH_BYTE_REGISTER);
	outb(CURSOR_REGISTER_DATA_PORT, ((uint16_t)pos >> 8) & 0xFF);
}
