#include "vbc.h"
// this is just for you to check the parsing
void	print_ast(node *n)
{
	if (!n)
		return ;
	if (n->type == VAL)
		printf("%d", n->val);
	else
	{
		printf("(");
		print_ast(n->l);
		if (n->type == ADD)
			printf(" + ");
		else if (n->type == MULTI)
			printf(" * ");
		else if (n->type == SUB)
			printf(" - ");
		print_ast(n->r);
		printf(")");
	}
}