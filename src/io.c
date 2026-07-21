#include <io.h>

static inline unsigned char inb(unsigned short port)
{
	unsigned char value;

	__asm__ volatile (
		"inb %1, %0"
		: "=a"(value)
		: "Nd"(port)
	);

	return value;
}
