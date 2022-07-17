/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 22:20:22 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/17 16:57:09 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "helper.h"
#include "../../include/lexer.h"

#define COUNT (25)

const char	*get_token_type(t_token_type type)
{
	if (type == E_TYPE_DEFAULT)
		return ("?");
	else if (type == E_TYPE_SIMPLE_CMD)
		return ("CMD_OR_ARG");
	/** else if (type == E_TYPE_CMD_OPTION) */
		/** return ("CMD_OPTION"); */
	else if (type == E_TYPE_SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == E_TYPE_DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	else if (type == E_TYPE_PIPE)
		return ("PIPE");
	else if (type == E_TYPE_REDIRECT)
		return ("REDIRECT");
	else if (type == E_TYPE_REDIR_ARG)
		return ("REDIR_ARG");
	else if (type == E_TYPE_BRACKET)
		return ("BRACKET");
	else if (type == E_TYPE_DOUBLE_AMPERSAND)
		return ("DOUBLE_AMPERSAND");
	else if (type == E_TYPE_DOUBLE_PIPE)
		return ("DOUBLE_PIPE");
	else
		return ("\033[31mERROR\033[0m");
}

void	print_tokens(t_list *tokens)
{
	t_list	*tmp;

	tmp = tokens;

	printf("\n");
	printf("--------------------------------\n");
	printf("|     Tokenizer Result         |\n");
	printf("--------------------------------\n");
	if (tmp == NULL)
		printf("No data\n");
	while (tmp != NULL)
	{
		t_token *tok = tmp->content;
		printf("\033[93m%20s $\033[0m", get_token_type(tok->type));
		printf("%s\n", tok->str->text);
		tmp = tmp->next;
	}
}

void	print_tree_node(t_list *token)
{
	t_list	*tmp;

	if (token == NULL)
		printf("\033[90m[null]\033[0m");
	tmp = token;
	while (tmp != NULL)
	{
		t_token *tok = tmp->content;
		printf("\033[92m[%s]\033[0m", tok->str->text);
		tmp = tmp->next;
	}
}

void	print_whitespace(int count, int space)
{
	int		i;

	i = count - 1;
	while (++i < space)
		printf(" ");
}

void	print_each_attr(t_list *tokens, int space, const char *messege)
{
	print_whitespace(COUNT, space);
	printf("\033[92m%s\033[0m", messege);
	print_tree_node(tokens);
	printf("\n");
}

/** tree print utils (for checking) */
void	print_tree_recur(t_tree *root, int space)
{
	if (root == NULL)
		return ;
	space += COUNT;
	print_tree_recur(root->right, space);

	print_whitespace(COUNT, space);
	if (root->token != NULL)
		printf("type : %s\n", get_token_type(((t_token *)root->token->content)->type));
	else
		printf("type : E_TYPE_DEFAULT\n");
	print_each_attr(root->token, space, "token: ");
	print_each_attr(root->redirection, space, "redir: ");
	printf("\n\n");

	print_tree_recur(root->left, space);
}

void	print_tree(t_tree *root)
{
	printf("\n");
	printf("--------------------------------------------------------\n");
	printf("|                Abstract Syntax Tree                  |\n");
	printf("--------------------------------------------------------\n");
	printf("\n");
	if (root == NULL)
		printf("No data\n");
	print_tree_recur(root, 0);
}
