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
	if (size < 0)
		return -1;

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

	size = print_hexa(nb >> 4);
	if (size < 0)
		return -1;

	ret = putchar(digits[nb & 0xF]);
	if (ret < 0)
		return -1;

	return size + ret;
}

static int print_hexa_width(uint32_t nb, int width)
{
	int	size;
	int	i;

	size = 0;

	/* Compte le nombre de chiffres hexadécimaux nécessaires. */
	if (nb == 0)
		size = 1;
	else
	{
		uint32_t	tmp;

		tmp = nb;
		while (tmp)
		{
			size++;
			tmp >>= 4;
		}
	}

	/* Padding avec des '0'. */
	i = size;
	while (i < width)
	{
		if (putchar('0') < 0)
			return -1;
		i++;
	}

	/* Affiche ensuite la valeur. */
	if (print_hexa(nb) < 0)
		return -1;

	if (size < width)
		return width;

	return size;
}

static int find_format(va_list *list, char c, int width, int zero_pad)
{
	(void)zero_pad;

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
			/*
			 * Pour l'instant, on garde le comportement
			 * original pour les entiers.
			 */
			return print_integer(va_arg(*list, int));

		case 'u':
			return print_unsigned(va_arg(*list, unsigned int));

		case 'x':
			if (width > 0)
				return print_hexa_width(
					va_arg(*list, unsigned int), width);
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
	int		width;
	int		zero_pad;

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

			/*
			 * Parse le padding :
			 *
			 * %x
			 * %02x
			 * %08x
			 *
			 * On reconnaît uniquement 0 + largeur.
			 */
			width = 0;
			zero_pad = 0;

			if (*fmt == '0')
			{
				zero_pad = 1;
				fmt++;

				while (*fmt >= '0' && *fmt <= '9')
				{
					width = width * 10 + (*fmt - '0');
					fmt++;
				}
			}

			ret = find_format(&list, *fmt, width, zero_pad);
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
