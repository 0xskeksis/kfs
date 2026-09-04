#include "stdio.h"
#include "ctype.h"
#include <stdint.h>

#define HEX_TABLE "0123456789abcdef"

static void
hex_convert(uint8_t bytes){
	if (bytes < 15)
		printf("%c", HEX_TABLE[bytes]);
	else
		hex_convert(bytes / 16);
}

void
hexdump(uint32_t *buffer, uint32_t len)
{
    uint8_t *data = (uint8_t *)buffer;

    for (uint32_t i = 0; i < len; i += 16)
    {
        printf("%08x  ", i);

        for (uint32_t j = 0; j < 16; j++)
        {
            if (i + j < len)
                printf("%02x ", data[i + j]);
            else
                printf("   ");
        }

        printf(" |");

        for (uint32_t j = 0; j < 16 && i + j < len; j++)
        {
            if (isprint(data[i + j]))
                printf("%c", data[i + j]);
            else
                printf(".");
        }

        printf("|\n");
    }
}
void
print_stack(){
	uint32_t *esp;
	asm volatile("mov %%esp, %0" : "=r"(esp));
	hexdump(esp, 1024);
}
