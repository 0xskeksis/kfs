#pragma once

#include <vga.h>

typedef struct
{
	char *line;
	char *command;
	char *args;
	size_t command_size;
	size_t args_size;
}	t_command_parse;

typedef struct
{
	char *name;
	size_t name_len;
	char *help_str;
	char *usage;
	int (*command)(char *param);
}	t_command;

void getline(t_terminal *term, unsigned int row, char dest[VGA_WIDTH]);
void readline(t_terminal *term, char dest[VGA_WIDTH]);
void parse_line(char *line, t_command_parse *command, size_t prefix_len);
void print_command(t_command_parse *command);
void shell_execute(t_terminal *term);

__attribute__((noreturn))
void reboot_kernel();

int help_cmd(char *_);
int test_cmd(char *arg);

__attribute__((noreturn))
int halt_cmd(char *_);

__attribute__((noreturn))
int reboot_cmd(char *_);

#define DEF_CMD(name, len, usage, help_msg) {#name"\0", len, usage"\0", help_msg"\0", name##_cmd}

#define CMD_NUM 4
static t_command commands[] =
{
	DEF_CMD(help, 4, "help", "Display this message"),
	DEF_CMD(test, 4, "test [arguments]", "Execute the test function"),
	DEF_CMD(halt, 4, "halt", "Halt the kernel"),
	DEF_CMD(reboot, 6, "reboot", "Reboot the kernel"),
};
