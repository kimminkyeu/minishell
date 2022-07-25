/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_get_token_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:59:12 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 18:00:36 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

void	get_dollar_sign(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (ft_isspace(c) || is_meta_char(c) || c == '\"' || c == '\'')
			break ;
		tok->f_push_back(tok, scan->f_next(scan));
	}
}

void	get_bracket(t_token *tok, t_scanner *scan)
{
	int	count;
	int	c;

	count = 0;
	tok->type = E_TYPE_BRACKET;
	while (scan->f_has_next(scan))
	{
		c = scan->f_next(scan);
		tok->f_push_back(tok, c);
		if (c == '(')
			count++;
		else if (c == ')')
			count--;
		if (count == 0)
			break ;
	}
}

void	get_cmd_or_arg(t_token *tok, t_scanner *scan)
{
	char	c; 

	tok->type = E_TYPE_SIMPLE_CMD;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);

		if (c == '\'')
			get_single_quote(tok, scan);
		else if (c == '\"')
			get_double_quote(tok, scan);
		else if (c == '$')
			get_dollar_sign(tok, scan);
		else if (ft_isspace(c) == false && is_meta_char(c) == false)
			tok->f_push_back(tok, scan->f_next(scan));
		else
			break ;
	}
}
