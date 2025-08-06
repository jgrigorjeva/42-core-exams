#include "vbc.h"

// I added a node SUB (like a subshell or a subtree in this case). I use it to handle an expression in parentheses.

int	main(int argc, char **argv)
{
	node	*tree;

	if (argc != 2)
		return (1);
	if (check_validity(argv[1]))
		return (1);
	tree = parse_expr(argv[1]);
	if (!tree)
		return (1);
	// print_ast(tree);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
}

node	*parse_expr(char *s)
{
	return (parse_add(&s));
}

node	*parse_add(char **s)
{
	node	*left;
	node	*right;
	node	*add_node;

	if (!**s)
		return (NULL);
	left = parse_multi(s);
	while (**s == '+')
	{
		(*s)++;
		right = parse_multi(s);
		add_node = new_node(ADD);
		if (!add_node)
		{
			return (NULL);
		}
		add_node->l = left;
		add_node->r = right;
		left = add_node;
	}
	return (left);
}

node	*parse_val_or_sub(char **s)
{
	if (**s == '(')
		return (parse_sub(s));
	else
		return (parse_val(s));
}

node	*parse_multi(char **s)
{
	node	*left;
	node	*right;
	node	*multi_node;

	left = parse_val_or_sub(s);
	while (**s == '*')
	{
		(*s)++;
		right = parse_val_or_sub(s);
		multi_node = new_node(MULTI);
		if (!multi_node)
			return (NULL);
		multi_node->l = left;
		multi_node->r = right;
		left = multi_node;
	}
	return (left);
}

node	*parse_val(char **s)
{
	node	*node_val;

	// if (!*s)
	// 	return (NULL);
	node_val = new_node(VAL);
	node_val->val = **s - '0';
	node_val->l = NULL;
	node_val->r = NULL;
	(*s)++;
	return (node_val);
}

node	*parse_sub(char **s)
{
	node	*node_sub;
	node	*left;

	if (**s != '(')
		return (NULL);
	(*s)++;
	left = parse_add(s);
	(*s)++; // skip ')'
	node_sub = new_node(SUB);
	if (!node_sub)
		return (NULL);
	node_sub->l = left;
	node_sub->r = NULL;
	return (node_sub);
}

int	eval_tree(node *tree)
{
	switch (tree->type)
	{
	case ADD:
		return (eval_tree(tree->l) + eval_tree(tree->r));
	case MULTI:
		return (eval_tree(tree->l) * eval_tree(tree->r));
	case VAL:
		return (tree->val);
	case SUB:
		return (eval_tree(tree->l));
	}
}