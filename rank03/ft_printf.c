#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

int ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
	{
		i++;
	}
	return (i);
}
int print_str(char *str)
{
	int ch_printed;
	int i;

	i = 0;
	ch_printed = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
		ch_printed++;
	}
	return (ch_printed);
}

void print_dec(int num, int *ch_printed)
{
	char digit;

	if (num == INT_MIN)
	{
		write(1, "-2147483648", 11);
        *ch_printed += 11;
        return;
	}
	if (num < 0)
	{
		num = -num;
		write(1, "-", 1);
		(*ch_printed)++;
	}
	if (num >= 10)
	{
		print_dec(num / 10, ch_printed);
		print_dec(num % 10, ch_printed);
	}
	else
	{
		digit = num + '0';
		write(1, &digit, 1);
		(*ch_printed)++;
	}
}

void print_hex(unsigned int num, int *ch_printed)
{
	char digit;

	if (num >= 16)
	{
		print_hex(num / 16, ch_printed);
		print_hex(num % 16, ch_printed);
	}
	else
	{
		if (num < 10)
			digit = num + '0';
		else
			digit = num - 10 + 'a';
		write(1, &digit, 1);
		(*ch_printed)++;
	}
}

int ft_printf(char *str, ...)
{
	va_list args;
	int i;
	int ch_printed;

	i = 0;
	ch_printed = 0;
	va_start(args, str);
	while (i < ft_strlen(str))
	{
		while (i < ft_strlen(str) && str[i] != '%')
		{
			write(1, &str[i], 1);
			i++;
			ch_printed++;
		}
		if (str[i] == '%')
		{
			i++;
			if (str[i] == 's')
				ch_printed += print_str(va_arg(args, char *));
			if (str[i] == 'd')
				print_dec(va_arg(args, int), &ch_printed);
			if (str[i] == 'x')
				print_hex(va_arg(args, unsigned int), &ch_printed);
			i++;
		}
	}
	va_end(args);
	return (ch_printed);
}

int main(void)
{
	int num1 = ft_printf("String: %s, decimal %d, hexadecimal %x\n", "abc", 2147483647, 4209);
	int num2 = printf("String: %s, decimal %d, hexadecimal %x\n", "abc", 2147483647, 4209);

	ft_printf("%d\n", num1);
	printf("%d\n", num2);

	return (1);
}