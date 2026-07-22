#include "kernel/vga.h"
#include "unistd.h"

ssize_t	write(int fd, const void *buf, size_t count){
	if (fd != 1 && fd != 2)
		return -1;
	terminal_write(buf, count);
	return count;
}
