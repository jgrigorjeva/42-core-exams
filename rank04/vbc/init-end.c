#include "vbc.h"

node	*new_node(t_type type)
{
	node	*ret;

	ret = calloc(1, sizeof(node));
	if (!ret)
		return (NULL);
	ret->type = type;
	return (ret);
}

void	destroy_tree(node *n)
{
	if (!n)
		return ;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}