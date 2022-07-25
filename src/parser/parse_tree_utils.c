/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 20:26:18 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 20:28:10 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_tree.h"

t_tree	*new_tree_node(void)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(*node));
	node->left = NULL;
	node->is_pipeline = 0;
	node->right = NULL;
	node->redirection = NULL;
	node->token = NULL;
	return (node);
}

/** Helper function for find_top_priority token */
t_list	*find_target_token(t_list *tokens, t_token_type _type)
{
	t_list	*cur;
	t_token	*token;

	cur = tokens;
	while (cur != NULL)
	{
		token = cur->content;
		if (_type == E_TYPE_REDIRECT \
				&& token->type >= 5 && token->type <= 8)
			return (cur);
		else if (token->type == _type)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

t_list	*find_top_priority_token(t_list *tokens)
{
	t_list	*target;
	int		i;

	i = 0;
	target = NULL;
	while (i < 5)
	{
		target = find_target_token(tokens, i);
		if (target != NULL)
			return (target);
		i++;
	}
	return (target);
}

t_list	*ft_lst_get_prev_node(t_list *node, t_list *target)
{
	t_list	*cur;

	if (node == NULL || target == NULL)
		return (NULL);
	if (node == target)
		return (NULL);
	cur = node;
	while (cur->next != NULL)
	{
		if (cur->next == target)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

/** function doesn't free data. just disconnect node from list */
void	ft_lst_pop_one(t_list **node, t_list *pop_target)
{
	t_list	*cur;

	if (node == NULL || *node == NULL || pop_target == NULL)
		return ;
	if (*node == pop_target)
	{
		*node = (*node)->next;
		return ;
	}
	cur = *node;
	while (cur != NULL)
	{
		if (cur->next == pop_target)
		{
			cur->next = pop_target->next;
			return ;
		}
		cur = cur->next;
	}
}
