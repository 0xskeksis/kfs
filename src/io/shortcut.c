#include "shell.h"
#include "vga.h"
#include <io/io.h>
#include <io/keyboard.h>
#include <io/shortcut.h>
#include <stdio.h>

#define DECLARE_COLOR_CHANGE_FN(name, fg, bg) int name() { CURRENT_TERMINAL->color = vga_entry_color(fg, bg); return 1;}
#define CURRENT_FG (CURRENT_TERMINAL->color & 0xFFFF)
#define CURRENT_BG (CURRENT_TERMINAL->color >> 4)

DECLARE_COLOR_CHANGE_FN(set_fg_white, VGA_COLOR_WHITE, CURRENT_BG)
DECLARE_COLOR_CHANGE_FN(set_fg_red, VGA_COLOR_RED, CURRENT_BG)
DECLARE_COLOR_CHANGE_FN(set_fg_green, VGA_COLOR_GREEN, CURRENT_BG)
DECLARE_COLOR_CHANGE_FN(set_fg_black, VGA_COLOR_BLACK, CURRENT_BG)
DECLARE_COLOR_CHANGE_FN(set_bg_black, CURRENT_FG, VGA_COLOR_BLACK)
DECLARE_COLOR_CHANGE_FN(set_bg_white, CURRENT_FG, VGA_COLOR_WHITE)
DECLARE_COLOR_CHANGE_FN(reset_color, VGA_COLOR_WHITE, VGA_COLOR_BLACK)

#undef DECLARE_COLOR_CHANGE_FN

int try_execute_shell()
{
	if (shell_mode == 1)
	{
		shell_execute(CURRENT_TERMINAL);
		return 1;
	}
	return 0;
}

int trigger_shell_mode()
{				
	shell_mode = !shell_mode;
	if (shell_mode == 1)
		printf("\n>");
	return 1;
}

int print_test()
{
	if (shell_mode == 0)
	{
		printf("\nTEST\n");
		return 1;
	}
	return 0;
}

int shortcut_flag_is_active(shortcut_flags flag)
{
	int res = 1;

	if (flag & SC_CTRL)
		res &= ctrl_is_active();
	
	if (flag & SC_ALT)
		res &= alt_is_active();

	if (flag & SC_SHIFT)
		res &= shift_is_active();

	return res;
}

int handle_shortcut(keycode key)
{
	for (size_t i = 0; i != SHORTCUTS_NUMBERS; i++)
	{
		t_shortcut current_sc = shortcuts[i];
		if (shortcut_flag_is_active(current_sc.sc_flags) && key == current_sc.sc_key)
			current_sc.sc_action();
	}
}
