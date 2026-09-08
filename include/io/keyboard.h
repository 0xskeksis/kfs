#pragma once

#include <stdint.h>

typedef enum 
{
	KBSF_CLEAR = 0,

	KBSF_LEFT_SHIFT = 1 << 0,
	KBSF_RIGHT_SHIFT = 1 << 1,

	KBSF_LEFT_ALT = 1 << 2,
	KBSF_RIGHT_ALT = 1 << 3,

	KBSF_LEFT_CTRL = 1 << 4,
	KBSF_RIGHT_CTRL = 1 << 5,

	KBSF_CAPS_LOCK = 1 << 6,

	KBSF_EXTENDED = 1 << 7
}	keyboard_state_flag;

char alt_is_active();
char ctrl_is_active();
char shift_is_active();
char caps_lock_is_active();
char extended_is_active();

typedef enum
{
	KEY_NONE = 0,

	/* Letters */
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	/* Number row */
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	/*
	 * Punctuation-key positions.
	 *
	 * Their generated character depends on Shift and the active
	 * keyboard layout.
	 */
	KEY_GRAVE,          /* `  ~ */
	KEY_MINUS,          /* -  _ */
	KEY_EQUAL,          /* =  + */
	KEY_LEFT_BRACKET,   /* [  { */
	KEY_RIGHT_BRACKET,  /* ]  } */
	KEY_BACKSLASH,      /* \  | */
	KEY_SEMICOLON,      /* ;  : */
	KEY_APOSTROPHE,     /* '  " */
	KEY_COMMA,          /* ,  < */
	KEY_PERIOD,         /* .  > */
	KEY_SLASH,          /* /  ? */

	/* Basic control keys */
	KEY_ESCAPE,
	KEY_TAB,
	KEY_CAPS_LOCK,
	KEY_ENTER,
	KEY_BACKSPACE,
	KEY_SPACE,

	/* Modifiers */
	KEY_LEFT_SHIFT,
	KEY_RIGHT_SHIFT,

	KEY_LEFT_CTRL,
	KEY_RIGHT_CTRL,

	KEY_LEFT_ALT,
	KEY_RIGHT_ALT,

	/* Wont be supported for now */
	KEY_LEFT_SUPER,     /* Windows / Command / Meta key */
	KEY_RIGHT_SUPER,
	KEY_MENU,           /* Application/context-menu key */

	/* Function keys */
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	/* System and lock keys, wont support them for now */
	KEY_PRINT_SCREEN,
	KEY_SCROLL_LOCK,
	KEY_PAUSE,
	KEY_NUM_LOCK,

	/* Navigation block */
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,

	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,

	/* Numeric keypad */
	KEY_KP_0,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,

	KEY_KP_DECIMAL,
	KEY_KP_DIVIDE,
	KEY_KP_MULTIPLY,
	KEY_KP_SUBTRACT,
	KEY_KP_ADD,
	KEY_KP_ENTER,

	KEY_COUNT
} keycode;

typedef struct
{
	keycode key;
	char pressed;
	char character;
} key_event;


int keyboard_read_scancode(unsigned char *scancode);
int keyboard_decode_byte(uint8_t byte, key_event *event);
void keyboard_update_state(keycode key, char pressed);
char keycode_to_char(keycode key);
void handle_keyboard_entry();

static const keycode normal_scancode_map[128] = {
	[0x01] = KEY_ESCAPE,

	[0x02] = KEY_1,
	[0x03] = KEY_2,
	[0x04] = KEY_3,
	[0x05] = KEY_4,
	[0x06] = KEY_5,
	[0x07] = KEY_6,
	[0x08] = KEY_7,
	[0x09] = KEY_8,
	[0x0A] = KEY_9,
	[0x0B] = KEY_0,

	[0x0C] = KEY_MINUS,
	[0x0D] = KEY_EQUAL,
	[0x0E] = KEY_BACKSPACE,
	[0x0F] = KEY_TAB,

	[0x10] = KEY_Q,
	[0x11] = KEY_W,
	[0x12] = KEY_E,
	[0x13] = KEY_R,
	[0x14] = KEY_T,
	[0x15] = KEY_Y,
	[0x16] = KEY_U,
	[0x17] = KEY_I,
	[0x18] = KEY_O,
	[0x19] = KEY_P,

	[0x1A] = KEY_LEFT_BRACKET,
	[0x1B] = KEY_RIGHT_BRACKET,
	[0x1C] = KEY_ENTER,
	[0x1D] = KEY_LEFT_CTRL,

	[0x1E] = KEY_A,
	[0x1F] = KEY_S,
	[0x20] = KEY_D,
	[0x21] = KEY_F,
	[0x22] = KEY_G,
	[0x23] = KEY_H,
	[0x24] = KEY_J,
	[0x25] = KEY_K,
	[0x26] = KEY_L,

	[0x27] = KEY_SEMICOLON,
	[0x28] = KEY_APOSTROPHE,
	[0x29] = KEY_GRAVE,
	[0x2A] = KEY_LEFT_SHIFT,
	[0x2B] = KEY_BACKSLASH,

	[0x2C] = KEY_Z,
	[0x2D] = KEY_X,
	[0x2E] = KEY_C,
	[0x2F] = KEY_V,
	[0x30] = KEY_B,
	[0x31] = KEY_N,
	[0x32] = KEY_M,

	[0x33] = KEY_COMMA,
	[0x34] = KEY_PERIOD,
	[0x35] = KEY_SLASH,
	[0x36] = KEY_RIGHT_SHIFT,

	[0x37] = KEY_KP_MULTIPLY,
	[0x38] = KEY_LEFT_ALT,
	[0x39] = KEY_SPACE,
	[0x3A] = KEY_CAPS_LOCK,

	[0x3B] = KEY_F1,
	[0x3C] = KEY_F2,
	[0x3D] = KEY_F3,
	[0x3E] = KEY_F4,
	[0x3F] = KEY_F5,
	[0x40] = KEY_F6,
	[0x41] = KEY_F7,
	[0x42] = KEY_F8,
	[0x43] = KEY_F9,
	[0x44] = KEY_F10,

	[0x45] = KEY_NUM_LOCK,
	[0x46] = KEY_SCROLL_LOCK,

	[0x47] = KEY_KP_7,
	[0x48] = KEY_KP_8,
	[0x49] = KEY_KP_9,
	[0x4A] = KEY_KP_SUBTRACT,
	[0x4B] = KEY_KP_4,
	[0x4C] = KEY_KP_5,
	[0x4D] = KEY_KP_6,
	[0x4E] = KEY_KP_ADD,
	[0x4F] = KEY_KP_1,
	[0x50] = KEY_KP_2,
	[0x51] = KEY_KP_3,
	[0x52] = KEY_KP_0,
	[0x53] = KEY_KP_DECIMAL,

	[0x57] = KEY_F11,
	[0x58] = KEY_F12,
};

static const keycode extended_scancode_map[128] = {
	[0x1C] = KEY_KP_ENTER,
	[0x1D] = KEY_RIGHT_CTRL,

	[0x35] = KEY_KP_DIVIDE,
	[0x38] = KEY_RIGHT_ALT,

	[0x47] = KEY_HOME,
	[0x48] = KEY_UP,
	[0x49] = KEY_PAGE_UP,

	[0x4B] = KEY_LEFT,
	[0x4D] = KEY_RIGHT,

	[0x4F] = KEY_END,
	[0x50] = KEY_DOWN,
	[0x51] = KEY_PAGE_DOWN,
	[0x52] = KEY_INSERT,
	[0x53] = KEY_DELETE,

	[0x5B] = KEY_LEFT_SUPER,
	[0x5C] = KEY_RIGHT_SUPER,
	[0x5D] = KEY_MENU,
};

static const char keycode_char_normal[KEY_COUNT] = {
	/* Letters */
	[KEY_A] = 'a',
	[KEY_B] = 'b',
	[KEY_C] = 'c',
	[KEY_D] = 'd',
	[KEY_E] = 'e',
	[KEY_F] = 'f',
	[KEY_G] = 'g',
	[KEY_H] = 'h',
	[KEY_I] = 'i',
	[KEY_J] = 'j',
	[KEY_K] = 'k',
	[KEY_L] = 'l',
	[KEY_M] = 'm',
	[KEY_N] = 'n',
	[KEY_O] = 'o',
	[KEY_P] = 'p',
	[KEY_Q] = 'q',
	[KEY_R] = 'r',
	[KEY_S] = 's',
	[KEY_T] = 't',
	[KEY_U] = 'u',
	[KEY_V] = 'v',
	[KEY_W] = 'w',
	[KEY_X] = 'x',
	[KEY_Y] = 'y',
	[KEY_Z] = 'z',

	/* Number row */
	[KEY_0] = '0',
	[KEY_1] = '1',
	[KEY_2] = '2',
	[KEY_3] = '3',
	[KEY_4] = '4',
	[KEY_5] = '5',
	[KEY_6] = '6',
	[KEY_7] = '7',
	[KEY_8] = '8',
	[KEY_9] = '9',

	/* Symbols */
	[KEY_GRAVE]         = '`',
	[KEY_MINUS]         = '-',
	[KEY_EQUAL]         = '=',
	[KEY_LEFT_BRACKET]  = '[',
	[KEY_RIGHT_BRACKET] = ']',
	[KEY_BACKSLASH]     = '\\',
	[KEY_SEMICOLON]     = ';',
	[KEY_APOSTROPHE]    = '\'',
	[KEY_COMMA]         = ',',
	[KEY_PERIOD]        = '.',
	[KEY_SLASH]         = '/',

	/* Control characters */
	[KEY_TAB]      = '\t',
	[KEY_ENTER]    = '\n',
	[KEY_BACKSPACE] = '\b',
	[KEY_SPACE]    = ' ',

	/* Numeric keypad */
	[KEY_KP_0] = '0',
	[KEY_KP_1] = '1',
	[KEY_KP_2] = '2',
	[KEY_KP_3] = '3',
	[KEY_KP_4] = '4',
	[KEY_KP_5] = '5',
	[KEY_KP_6] = '6',
	[KEY_KP_7] = '7',
	[KEY_KP_8] = '8',
	[KEY_KP_9] = '9',

	[KEY_KP_DECIMAL]  = '.',
	[KEY_KP_DIVIDE]   = '/',
	[KEY_KP_MULTIPLY] = '*',
	[KEY_KP_SUBTRACT] = '-',
	[KEY_KP_ADD]      = '+',
	[KEY_KP_ENTER]    = '\n',
};

static const char keycode_char_shifted[KEY_COUNT] = {
	/* Letters */
	[KEY_A] = 'A',
	[KEY_B] = 'B',
	[KEY_C] = 'C',
	[KEY_D] = 'D',
	[KEY_E] = 'E',
	[KEY_F] = 'F',
	[KEY_G] = 'G',
	[KEY_H] = 'H',
	[KEY_I] = 'I',
	[KEY_J] = 'J',
	[KEY_K] = 'K',
	[KEY_L] = 'L',
	[KEY_M] = 'M',
	[KEY_N] = 'N',
	[KEY_O] = 'O',
	[KEY_P] = 'P',
	[KEY_Q] = 'Q',
	[KEY_R] = 'R',
	[KEY_S] = 'S',
	[KEY_T] = 'T',
	[KEY_U] = 'U',
	[KEY_V] = 'V',
	[KEY_W] = 'W',
	[KEY_X] = 'X',
	[KEY_Y] = 'Y',
	[KEY_Z] = 'Z',

	/* Shifted number row */
	[KEY_0] = ')',
	[KEY_1] = '!',
	[KEY_2] = '@',
	[KEY_3] = '#',
	[KEY_4] = '$',
	[KEY_5] = '%',
	[KEY_6] = '^',
	[KEY_7] = '&',
	[KEY_8] = '*',
	[KEY_9] = '(',

	/* Shifted symbols */
	[KEY_GRAVE]         = '~',
	[KEY_MINUS]         = '_',
	[KEY_EQUAL]         = '+',
	[KEY_LEFT_BRACKET]  = '{',
	[KEY_RIGHT_BRACKET] = '}',
	[KEY_BACKSLASH]     = '|',
	[KEY_SEMICOLON]     = ':',
	[KEY_APOSTROPHE]    = '"',
	[KEY_COMMA]         = '<',
	[KEY_PERIOD]        = '>',
	[KEY_SLASH]         = '?',

	/*
	 * Shift does not change these control characters.
	 */
	[KEY_TAB]       = '\t',
	[KEY_ENTER]     = '\n',
	[KEY_BACKSPACE] = '\b',
	[KEY_SPACE]     = ' ',

	/*
	 * Numeric keypad operators generally do not change with Shift.
	 *
	 * Keypad digit behavior should be controlled using Num Lock,
	 * rather than this shifted table.
	 */
	[KEY_KP_0] = '0',
	[KEY_KP_1] = '1',
	[KEY_KP_2] = '2',
	[KEY_KP_3] = '3',
	[KEY_KP_4] = '4',
	[KEY_KP_5] = '5',
	[KEY_KP_6] = '6',
	[KEY_KP_7] = '7',
	[KEY_KP_8] = '8',
	[KEY_KP_9] = '9',

	[KEY_KP_DECIMAL]  = '.',
	[KEY_KP_DIVIDE]   = '/',
	[KEY_KP_MULTIPLY] = '*',
	[KEY_KP_SUBTRACT] = '-',
	[KEY_KP_ADD]      = '+',
	[KEY_KP_ENTER]    = '\n',
};
