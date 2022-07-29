/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 14:21:40 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 16:50:46 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_tree.h"

#define NEED_FORK			(0)
#define IS_LAST_PIPE_CMD	(1)

t_list	*collect_redirection_node(t_list *tokens)
{
	t_list	*target;
	t_list	*redir_list;
	t_list	*cur;

	target = find_target_token(tokens, E_TYPE_REDIRECT);
	redir_list = target;
	cur = redir_list;
	ft_lst_pop_one(&tokens, target);
	while (target != NULL && target->next != NULL)
	{
		if (target != NULL && target->next != NULL \
				&& is_type_redir_arg(target->next))
		{
			target = target->next;
			cur->next = target;
			ft_lst_pop_one(&tokens, target);
			cur = cur->next;
		}
		target = find_target_token(tokens, E_TYPE_REDIRECT);
		cur->next = target;
		ft_lst_pop_one(&tokens, target);
		cur = cur->next;
	}
	return (redir_list);
}

/** Parse to tree function's divided for NORMINETTE */
void	cut_node_and_do_recur(t_tree *parent, t_list *tokens, \
		t_list *target_token, int *flag)
{
	t_list	*left_tokens;
	t_list	*right_tokens;
	t_list	*target_prev;
	t_token	*target_token_ptr;

	left_tokens = tokens;
	right_tokens = target_token->next;
	target_prev = ft_lst_get_prev_node(tokens, target_token);
	target_token_ptr = target_token->content;
	if (target_prev != NULL)
		target_prev->next = NULL;
	target_token->next = NULL;
	if (target_token_ptr->type == E_TYPE_PIPE)
	{
		flag[NEED_FORK] = 1;
		flag[IS_LAST_PIPE_CMD] = 0;
	}
	parent->left = parse_to_tree_recur(left_tokens, flag);
	if (target_token_ptr->type == E_TYPE_PIPE)
		flag[IS_LAST_PIPE_CMD] = 1;
	if (target_token_ptr->type == E_TYPE_DOUBLE_AMPERSAND \
			|| target_token_ptr->type == E_TYPE_DOUBLE_PIPE)
		flag[NEED_FORK] = 0;
	parent->right = parse_to_tree_recur(right_tokens, flag);
}

/** NOTE : Error log.
 *  if command is [ <infile ], then <infile should be
 *  the last pipe cmd of a cmd-cycle.
 *
 *  [ target_token_ptr == NULL ] means node has only redirection. (no command)
 *  ex. [ <in ] or [ <in | cat ]
 * */
void	set_node_flag(t_tree *parent, t_token *target_token_ptr, int *flag)
{
	if (flag[NEED_FORK] == 1 && target_token_ptr == NULL)
	{
		parent->is_pipeline = 1;
		if (flag[IS_LAST_PIPE_CMD] == 1)
			parent->is_last_pipe_cmd = 1;
	}
	else if (flag[NEED_FORK] == 0 && target_token_ptr == NULL)
	{
		parent->is_last_pipe_cmd = 1;
	}
	else if (flag[NEED_FORK] == 1 && \
			(target_token_ptr->type == E_TYPE_SIMPLE_CMD \
			|| target_token_ptr->type == E_TYPE_BRACKET))
	{
		parent->is_pipeline = 1;
		if (flag[IS_LAST_PIPE_CMD] == 1)
			parent->is_last_pipe_cmd = 1;
	}
	else if (flag[NEED_FORK] == 0 && \
			(target_token_ptr->type == E_TYPE_SIMPLE_CMD \
			|| target_token_ptr->type == E_TYPE_BRACKET))
	{
		parent->is_last_pipe_cmd = 1;
	}
}

t_tree	*parse_to_tree_recur(t_list *tokens, int *flag)
{
	t_tree	*parent;
	t_list	*target_token;
	t_token	*target_token_ptr;

	if (tokens == NULL)
		return (NULL);
	parent = new_tree_node();
	target_token = find_top_priority_token(tokens);
	parent->token = target_token;
	target_token_ptr = NULL;
	if (target_token != NULL)
		target_token_ptr = target_token->content;
	set_node_flag(parent, target_token_ptr, flag);
	if (target_token == NULL || target_token_ptr->type == E_TYPE_SIMPLE_CMD \
			|| target_token_ptr->type == E_TYPE_BRACKET)
		parent->redirection = collect_redirection_node(tokens);
	else
		cut_node_and_do_recur(parent, tokens, target_token, flag);
	return (parent);
}

t_tree	*parse(t_list *tokens)
{
	int	flag[2];

	flag[NEED_FORK] = 0;
	flag[IS_LAST_PIPE_CMD] = 0;
	return (parse_to_tree_recur(tokens, flag));
}
