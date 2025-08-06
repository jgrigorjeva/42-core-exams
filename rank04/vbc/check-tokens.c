#include "vbc.h"

int	check_validity(char *s)
{
	if (check_rest(s) || check_parentheses(s)) // one of the test cases mixes invalid number with invalid parentheses combination. The invalid digits ('12') come first, so the error message should first come from that
		return (1);
	return 0;
}

int check_parentheses(char *s)
{
	int left = 0;
	int right = 0;

	while (*s)
	{
		if (*s ==  '(')
			left++;
		if (*s == ')')
			right++;
		if (right > left)
		{
			printf("Unexpected token ')'\n");
			return (1);
		}
		s++;
	}
	if (left > right)
	{
		printf("Unexpected token '('\n");
		return (1);
	}
	return 0;
}

int check_rest(char *s)
{
	while (*s && *(s+1))
	{
		if (is_operator(*s) && is_operator(*(s+1)))
		{
			unexpected(*(s+1));
			return (1);
		}
			
		if (isdigit(*s) && isdigit(*(s+1)))
		{
			unexpected(*(s+1));
			return (1);
		}
		if (token_invalid(*s))
		{
			unexpected(*s);
			return (1);
		}
		s++;
	}
	if (is_operator(*s))
	{
		unexpected('\0');
		return (1);
	}
	if (token_invalid(*s))
	{
		unexpected(*s);
		return (1);
	}
	return (0);
}

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int is_operator(char c)
{
	return (c == '+' || c == '*');
}

int token_invalid(char c)
{
	return (!(is_operator(c) || isdigit(c) || c == '(' || c == ')'));
}
