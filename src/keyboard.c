#include <io.h>
#include <keyboard.h>
#include <stdint.h>

static unsigned char keyboard_state = 0;

#define PS_2_READING_STATUS_REGISTER 0x64
#define PS_2_READING_DATA_REGISTER 0x60

#define KEY_RELEASED_FLAG 0x80

int keyboard_read_sancode(unsigned char *scancode)
{
	if ((inb(PS_2_READING_STATUS_REGISTER) & 1) == 0)
		return 0;

	*scancode = inb(PS_2_READING_DATA_REGISTER);
	return 1;
}

int keyboard_decode_byte(uint8_t byte, key_event *event)
{
	if (byte == 0xE0)
	{
		keyboard_state |= KBSF_EXTENDED;
		return 0;
	}

	char released = (byte & KEY_RELEASED_FLAG) != 0;
	
}
