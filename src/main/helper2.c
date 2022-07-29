/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:05:30 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 16:17:49 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "helper.h"
#include "../../include/lexer.h"

#define COUNT (25)

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
		printf("type : %s\n", \
				get_token_type(((t_token *)root->token->content)->type));
	else
		printf("type : E_TYPE_DEFAULT\n");
	print_each_attr(root->token, space, "token: ");
	print_each_attr(root->redirection, space, "redir: ");
	print_whitespace(COUNT, space);
	printf("is_pipe : %d\n", root->is_pipeline);
	print_whitespace(COUNT, space);
	printf("last_pipe_cmd : %d\n", root->is_last_pipe_cmd);
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
