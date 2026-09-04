#include <io/io.h>
#include <io/keyboard.h>
#include <stdio.h>
#include <vga.h>
#include <stdint.h>
#include <shell.h>

static unsigned char keyboard_state = 0;

#define PS_2_READING_STATUS_REGISTER 0x64
#define PS_2_READING_DATA_REGISTER 0x60

#define KEY_RELEASED_FLAG 0x80

#define EXTENDED_SEQUENCE_CODE 0xE0

int keyboard_read_scancode(unsigned char *scancode)
{
	if ((inb(PS_2_READING_STATUS_REGISTER) & 1) == 0)
		return 0;

	*scancode = inb(PS_2_READING_DATA_REGISTER);
	return 1;
}

static inline char alt_is_active()
{
	return (keyboard_state & (KBSF_LEFT_ALT | KBSF_RIGHT_ALT)) != 0;
}

static inline char ctrl_is_active()
{
	return (keyboard_state & (KBSF_LEFT_CTRL | KBSF_RIGHT_CTRL)) != 0;
}

static inline char shift_is_active()
{
	return (keyboard_state & (KBSF_LEFT_SHIFT | KBSF_RIGHT_SHIFT)) != 0;
}

static inline char caps_lock_is_active()
{
	return (keyboard_state & KBSF_CAPS_LOCK) != 0;
}

static inline char extended_is_active()
{
	return (keyboard_state & KBSF_EXTENDED) != 0;
}

int keyboard_decode_byte(uint8_t byte, key_event *event)
{
	if (event == 0)
		return 0;

	if (byte == EXTENDED_SEQUENCE_CODE)
	{
		keyboard_state |= KBSF_EXTENDED;
		return 0;
	}

	char extended = extended_is_active();
	
	keyboard_state &= (uint8_t)~KBSF_EXTENDED;

	char released = (byte & KEY_RELEASED_FLAG) != 0;

	uint8_t scancode = byte & (uint8_t)~KEY_RELEASED_FLAG;

	keycode key;

	if (extended)
		key = extended_scancode_map[scancode];
	else
		key = normal_scancode_map[scancode];

	if (key == KEY_NONE)
		return 0;

	event->key = key;
	event->pressed = !released;
	event->character = '\0';

	return 1;
}

#define HANDLE_KEY_STATE(key)						\
{													\
	case KEY_##key:									\
		if (pressed)								\
			keyboard_state |= KBSF_##key;			\
		else										\
			keyboard_state &= (uint8_t)~KBSF_##key;	\
		break;										\
}

void keyboard_update_state(keycode key, char pressed)
{
	switch(key)
	{
		HANDLE_KEY_STATE(LEFT_SHIFT);
		HANDLE_KEY_STATE(RIGHT_SHIFT);
		HANDLE_KEY_STATE(LEFT_ALT);
		HANDLE_KEY_STATE(RIGHT_ALT);
		HANDLE_KEY_STATE(LEFT_CTRL);
		HANDLE_KEY_STATE(RIGHT_CTRL);
		
		case KEY_CAPS_LOCK:
			if (pressed)
				keyboard_state ^= KBSF_CAPS_LOCK;
			break;

		default:
			break;
	}
}

char keycode_to_char(keycode key)
{
	char normal;

	if (key <= KEY_NONE || key >= KEY_COUNT)
		return '\0';

	normal = keycode_char_normal[key];

	if (normal >= 'a' && normal <= 'z')
	{
		if (shift_is_active() != caps_lock_is_active())
			return keycode_char_shifted[key];

		return normal;
	}

	if (shift_is_active())
		return keycode_char_shifted[key];

	return normal;
}

#undef HANDLE_KEY_STATE

#define KEY_TO_COLOR_CASE(key, fg, bg) case key: CURRENT_TERMINAL->color = vga_entry_color(fg, bg); return 1

int handle_shortcut(keycode key)
{
	if (key == KEY_ENTER)
	{
		if (shell_mode == 1)
		{
			shell_execute(CURRENT_TERMINAL);
			return 1;
		}
	}
	if (ctrl_is_active())
	{
		switch (key)
		{
			KEY_TO_COLOR_CASE(KEY_1, VGA_COLOR_WHITE, CURRENT_TERMINAL->color >> 4);
			KEY_TO_COLOR_CASE(KEY_2, VGA_COLOR_RED, CURRENT_TERMINAL->color >> 4);
			KEY_TO_COLOR_CASE(KEY_3, VGA_COLOR_GREEN, CURRENT_TERMINAL->color >> 4);
			KEY_TO_COLOR_CASE(KEY_4, VGA_COLOR_BLACK, CURRENT_TERMINAL->color >> 4);
			KEY_TO_COLOR_CASE(KEY_5, CURRENT_TERMINAL->color & 0xFFFF, VGA_COLOR_BLACK);
			KEY_TO_COLOR_CASE(KEY_6, CURRENT_TERMINAL->color & 0xFFFF, VGA_COLOR_WHITE);
			KEY_TO_COLOR_CASE(KEY_7, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
			case KEY_S:
			{
				shell_mode = !shell_mode;
				if (shell_mode == 1)
					printf("\n>");
				return 1;
			}
			default:
				break;
		}
	}

	return 0;
}

#undef SWITCH_TERM
#undef KEY_TO_COLOR_CASE

void handle_keyboard_entry()
{
	unsigned char scancode;

	int res = keyboard_read_scancode(&scancode);
	if (res == 0)
		return;

	key_event event;

	res = keyboard_decode_byte(scancode, &event);
	if (res == 0)
		return;

	keyboard_update_state(event.key, event.pressed);
	if (!event.pressed)
		return;

	res = handle_shortcut(event.key);
	if (res == 1)
		return;

	char character = keycode_to_char(event.key);
	if (character == '\0')
		return;

	terminal_putchar(CURRENT_TERMINAL, character);
}
