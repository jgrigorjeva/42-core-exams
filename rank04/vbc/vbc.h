#ifndef VBC_H
# define VBC_H

#include <stdio.h>
#include <malloc.h>
#include <ctype.h>

typedef enum s_type
{
	ADD,
	MULTI,
	VAL,
	SUB
}   t_type;

typedef struct node
{
	t_type type;
    int val;
    struct node *l;
    struct node *r;
}   node;

void	destroy_tree(node *n);
node	*new_node(t_type type);
void	skipspace(char **s);
int		check_validity(char *s);
int 	check_parentheses(char *s);
int check_rest(char *s);
void	unexpected(char c);
int token_invalid(char c);
int is_operator(char c);

// parser
node	*parse_expr(char *s);
node *parse_multi(char **s);
node *parse_val_or_sub(char **s);
node *parse_add(char **s);
node	*parse_sub(char **s);
node	*parse_val(char **s);

// utils
void print_ast(node *n);

// eval
int	eval_tree(node *tree);


#endif