/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_helper2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 14:36:40 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/03 11:09:07 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "../../include/libft.h"
#include "../main/helper.h"
#include "scanner.h"

bool	is_bracket_has_no_bracket(char c, t_token *tok_1)
{
	if (c != ')' && c != '(')
	{
		tok_1->str->text[tok_1->str->text_len - 1] = '\0';
		print_error("lesh: syntax error near unexpected token", \
				tok_1->str->text + 1);
		return (true);
	}
	return (false);
}

bool	is_bracket_unclosed(t_token *tok_1)
{
	char	c;
	char	*tmp;

	c = tok_1->str->text[tok_1->str->text_len - 1];
	if ((tok_1->str->text[0] == '(' && c != ')'))
	{
		tok_1->str->text[tok_1->str->text_len - 1] = '\0';
		ft_putstr_fd("lesh: syntax error near unexpected token `newline'\n"\
				, STDERR_FILENO);
		return (true);
	}
	if (is_bracket_has_no_bracket(c, tok_1))
		return (true);
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

bool	is_meta_token_type(t_token_type type)
{
	if (type != E_TYPE_PIPE && type != E_TYPE_DOUBLE_PIPE && \
			type != E_TYPE_DOUBLE_AMPERSAND && type != E_TYPE_AMPERSAND)
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
