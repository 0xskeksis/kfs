#ifndef _VGA
# define _VGA

#define 	VGA_COLOR_BLACK  0
#define 	VGA_COLOR_BLUE  1
#define 	VGA_COLOR_GREEN  2
#define 	VGA_COLOR_CYAN  3
#define 	VGA_COLOR_RED  4
#define 	VGA_COLOR_MAGENTA  5
#define 	VGA_COLOR_BROWN  6
#define 	VGA_COLOR_LIGHT_GREY  7
#define 	VGA_COLOR_DARK_GREY  8
#define 	VGA_COLOR_LIGHT_BLUE  9
#define 	VGA_COLOR_LIGHT_GREEN  10
#define 	VGA_COLOR_LIGHT_CYAN  11
#define 	VGA_COLOR_LIGHT_RED  12
#define 	VGA_COLOR_LIGHT_MAGENTA  13
#define 	VGA_COLOR_LIGHT_BROWN  14
#define 	VGA_COLOR_WHITE  15

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

#include "stddef.h"
#include "stdint.h"
#include "unistd.h"

void terminal_writestring(const char* data);
void terminal_write(const char* data, size_t size);
void terminal_putchar(char c);
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);

ssize_t	write(int fd, const void *buf, size_t count);



#endif
