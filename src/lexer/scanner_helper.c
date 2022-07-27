/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:52:12 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 15:19:15 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "../../include/libft.h"
#include "../main/helper.h"
#include "scanner.h"

/** FIXME : if is_syntax_error doens't check properly, change here */
bool	is_meta_token_type(t_token_type type)
{
	if (type != E_TYPE_PIPE && type != E_TYPE_DOUBLE_PIPE && \
			type != E_TYPE_DOUBLE_AMPERSAND)
		return (false);
	else
		return (true);
}

bool	is_meta_char(char c)
{
	if (c != '|' && c != '&' && c != '<' && c != '>')
		return (false);
	else
		return (true);
}

bool	is_meta_token_overlap(t_list *token_list)
{
	t_list	*cur;
	t_token	*tok_1;
	t_token	*tok_2;
	char	c;

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

bool	is_bracket_unclosed(t_token *tok_1)
{
	char	c;
	char	*tmp;

	c = tok_1->str->text[tok_1->str->text_len - 1];
	if (c != ')' && c != '(')
	{
		tok_1->str->text[tok_1->str->text_len - 1] = '\0';
		print_error("lesh: syntax error near unexpected token", \
				tok_1->str->text + 1);
		return (true);
	}
	tmp = &(tok_1->str->text[1]);
	while (*tmp != '\0')
	{
		if (*tmp == '(')
		{
			tok_1->str->text[tok_1->str->text_len - 1] = '\0';
			print_error("lesh: syntax error near unexpected token", tmp + 1);
			return (true);
		}
		tmp++;
	}
	return (false);
}

/** if last token ends with meta character (| || &&) */
bool	is_last_token_meta(t_list *token_list)
{
	t_list	*cur;
	t_token	*tok_1;

	cur = token_list;
	while (cur->next != NULL)
		cur = cur->next;
	tok_1 = cur->content;
	if (tok_1->type == E_TYPE_REDIRECT)
	{
		ft_putstr_fd("lesh: syntax error near unexpected token \
				`newline'\n", STDERR_FILENO);
		return (true);
	}
	else if (is_meta_token_type(tok_1->type))
	{
		ft_putstr_fd("lesh: syntax error near unexpected \
				token \'", STDERR_FILENO);
		ft_putstr_fd(tok_1->str->text, STDERR_FILENO);
		ft_putstr_fd("\'\n", STDERR_FILENO);
		return (true);
	}
	else if (tok_1->type == E_TYPE_BRACKET && is_bracket_unclosed(tok_1))
		return (true);
	else
		return (false);
}
