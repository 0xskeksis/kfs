#include "stdarg.h"
#include "unistd.h"
#include "stdio.h"
#include "stdint.h"

static int print_unsigned(uint32_t nb)
{
	int size;

	if (nb < 10)
		return putchar((int)(nb + '0'));

	size = print_unsigned(nb / 10);
	if (putchar((int)(nb % 10 + '0')) < 0)
		return -1;

	return size + 1;
}

static int print_integer(int nb)
{
	uint32_t	value;
	int			size;
	int			ret;

	size = 0;
	if (nb < 0)
	{
		ret = putchar('-');
		if (ret < 0)
			return -1;
		size += ret;

		/* Évite le débordement pour INT_MIN. */
		value = (uint32_t)(-(nb + 1));
		value += 1;
	}
	else
		value = (uint32_t)nb;

	ret = print_unsigned(value);
	if (ret < 0)
		return -1;

	return size + ret;
}

static int print_hexa(uint32_t nb)
{
	static const char	digits[] = "0123456789abcdef";
	int					size;
	int					ret;

	if (nb < 16)
		return putchar(digits[nb]);

	/* Une division par 16 correspond à un décalage de 4 bits. */
	size = print_hexa(nb >> 4);
	if (size < 0)
		return -1;

	ret = putchar(digits[nb & 0xF]);
	if (ret < 0)
		return -1;

	return size + ret;
}

static int find_format(va_list *list, char c)
{
	switch (c)
	{
		case '%':
			return putchar('%');
		case 'c':
			return putchar(va_arg(*list, int));
		case 's':
			return putstr(va_arg(*list, char *));
		case 'i':
		case 'd':
			return print_integer(va_arg(*list, int));
		case 'u':
			return print_unsigned(va_arg(*list, unsigned int));
		case 'x':
			return print_hexa(va_arg(*list, unsigned int));
		default:
			return 0;
	}
}

int printf(const char *fmt, ...)
{
	va_list	list;
	int		count;
	int		ret;

	if (fmt == NULL)
		return -1;

	count = 0;
	va_start(list, fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			if (*fmt == '\0')
				break;
			ret = find_format(&list, *fmt);
		}
		else
			ret = putchar(*fmt);

		if (ret < 0)
		{
			va_end(list);
			return -1;
		}

		count += ret;
		fmt++;
	}

	va_end(list);
	return count;
}
