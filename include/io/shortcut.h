#pragma once

#include "io/keyboard.h"
#include <vga.h>

typedef enum
{
	SC_NONE		= 0,
	SC_CTRL		= 1 << 0,
	SC_ALT		= 1 << 1,
	SC_SHIFT	= 1 << 2
} shortcut_flags;

typedef struct
{
	shortcut_flags sc_flags;
	keycode sc_key;
	int (*sc_action)();
}	t_shortcut;

int try_execute_shell();
int trigger_shell_mode();

int set_fg_white();
int set_fg_red();
int set_fg_green();
int set_fg_black();
int set_bg_black();
int set_bg_white();
int reset_color();

int print_test();

static const t_shortcut shortcuts[] = 
{
	{SC_NONE, KEY_ENTER, try_execute_shell},
	{SC_CTRL, KEY_1, set_fg_white},
	{SC_CTRL, KEY_2, set_fg_red},
	{SC_CTRL, KEY_3, set_fg_green},
	{SC_CTRL, KEY_4, set_fg_black},
	{SC_CTRL, KEY_5, set_bg_black},
	{SC_CTRL, KEY_6, set_bg_white},
	{SC_CTRL, KEY_7, reset_color},
	{SC_CTRL, KEY_S, trigger_shell_mode},
	{SC_ALT | SC_SHIFT, KEY_1, print_test},
};

#define SHORTCUTS_NUMBERS (sizeof(shortcuts)/(sizeof(shortcuts[0])))

int handle_shortcut(keycode key);
