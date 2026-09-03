#include "vga.h"
#include <ctype.h>
#include <shell.h>
#include <string.h>
#include <stdio.h>

char *skip_whitespace(char *str)
{
	char *cpy = str;
	while (*cpy != '\0' && isspace(*cpy))
		cpy++;
	return cpy;
}

void parse_line(char *line, t_command_parse *command, size_t prefix_len)
{
	printf("\nLine info: str %s str + prefix %s\n", line, &(line[prefix_len]));
	command->line = &(line[prefix_len]);
	command->line = skip_whitespace(line);
	char *cpy = command->line;

	while (*cpy != '\0' && isalnum(*cpy))
		cpy++;

	command->command_size = cpy - command->line;
	cpy = skip_whitespace(cpy);
	if (*cpy == '\0')
	{
		command->args = NULL;
		command->args_size = 0;
		return;
	}
	command->args = cpy;
	command->args_size = strlen(cpy);
	return;
}

void print_command(t_command_parse *command)
{
	printf("\ncmd->line: %s\ncmd->args: %s\ncmd->command_size: %d\ncmd->args_size: %d\n", command->line, command->args, command->command_size, command->args_size);
}

int help_cmd(__attribute__((unused)) char *_)
{
	printf("Command usage:\n");

	for (int i = 0; i != CMD_NUM; i++)
		printf("- %s: %s\n", commands[i].usage, commands[i].help_str);
	return 0;
}

int test_cmd(char *args)
{
	printf("test command: %s\n", args);
	return 0;
}

void shell_execute(t_terminal *term)
{
	t_command_parse parsing;
	char line[VGA_WIDTH];

	readline(term, line);
	parse_line(line, &parsing, 1);
	printf("\n");

	int did_cmd = 0;

	for (int i = 0; i != CMD_NUM; i++)
	{
		t_command current = commands[i];
		int res = strncmp(parsing.command, current.name, current.name_len);
		printf("Current test: command vs line: %s; %s\nSize: %d, comp result: %d\n", parsing.command, current.name, current.name_len, res);
		if (!res)
		{
			current.command(parsing.args);
			did_cmd = 1;
		}
	}

	if (did_cmd == 0)
		printf("Command not found, use 'help' to see the list of commands and their usage.\n");
	printf(">");
}
