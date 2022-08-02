/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:52:12 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 17:48:53 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "../../include/libft.h"
#include "../main/helper.h"
#include "scanner.h"

bool	is_meta_token_overlap(t_list *token_list)
{
	t_list	*cur;
	t_token	*tok_1;
	t_token	*tok_2;
	char	c;

	if (token_list == NULL)
		return (false);
	cur = token_list;
	while (cur->next != NULL)
	{
		tok_1 = cur->content;
		tok_2 = cur->next->content;
		c = tok_2->str->text[0];
		if (is_meta_token_type(tok_1->type) && is_meta_token_type(tok_2->type))
		{
			print_error("lesh: syntax error near unexpected token", &c);
			return (true);
		}
		cur = cur->next;
	}
	return (false);
}

bool	is_last_token_meta_2(t_token *tok_1)
{
	if (tok_1->type == E_TYPE_REDIRECT)
	{
		ft_putstr_fd("lesh: syntax error near unexpected token `newline'\n"\
				, STDERR_FILENO);
		return (true);
	}
	else if (is_meta_token_type(tok_1->type))
	{
		ft_putstr_fd("lesh: syntax error near unexpected token \'"\
				, STDERR_FILENO);
		ft_putstr_fd(tok_1->str->text, STDERR_FILENO);
		ft_putstr_fd("\'\n", STDERR_FILENO);
		return (true);
	}
	if (tok_1->str->text[0] == ')')
	{
		ft_putstr_fd("lesh: syntax error near unexpected token `)'\n"\
				, STDERR_FILENO);
		return (true);
	}
	return (false);
}

/** if last token ends with meta character (| || &&) */
bool	is_last_token_meta(t_list *token_list)
{
	t_list	*cur;
	t_token	*tok_1;

	cur = token_list;
	while (cur != NULL && cur->next != NULL)
		cur = cur->next;
	tok_1 = cur->content;
	if (is_last_token_meta_2(tok_1))
		return (true);
	else if (tok_1->type == E_TYPE_BRACKET && is_bracket_unclosed(tok_1))
		return (true);
	else
		return (false);
}
