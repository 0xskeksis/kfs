#include "vga.h"
#include <ctype.h>
#include <shell.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

char *skip_whitespace(char *str)
{
	char *cpy = str;
	while (*cpy != '\0' && isspace(*cpy))
		cpy++;
	return cpy;
}

void parse_line(char *line, t_command_parse *command, size_t prefix_len)
{
	command->line = &(line[prefix_len]);
	command->line = skip_whitespace(command->line);
	command->command = command->line;
	char *cpy = command->line;


	while (*cpy != '\0' && (isalnum((unsigned char)*cpy) || *cpy == '_'))
		cpy++;

	command->command_size = cpy - command->line;

	if (*cpy == '\0')
	{
		command->args = NULL;
		command->args_size = 0;
		return;
	}

	*cpy = '\0';
	cpy++;
	cpy = skip_whitespace(cpy);
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
	if (args == NULL)
		printf("test command: test\n");
	else
		printf("test command: %s\n", args);
	return 0;
}

int halt_cmd(__attribute__((unused)) char*_)
{
	__asm__ volatile ("hlt"::);
	__builtin_unreachable();
}

int reboot_cmd(__attribute__((unused)) char *_)
{
	reboot_kernel();
}

int print_stack_cmd(char *size)
{
	if (size == NULL)
		goto error;

	uint32_t len = atoi(size);

	if (len <= 0)
		goto error;

	print_stack(len);
	return 0;

error:
	printf("Usage: print_stack [stack_size], with stack_size >= 1\n");
	return 1;

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
		if (!res && current.name_len == parsing.command_size)
		{
			current.command(parsing.args);
			did_cmd = 1;
			break;
		}
	}

	if (did_cmd == 0)
		printf("Command not found, use 'help' to see the list of commands and their usage.\n");
	printf(">");
}
