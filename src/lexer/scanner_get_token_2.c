/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_get_token_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:01:20 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 18:01:25 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

void	get_pipe(t_token *tok, t_scanner *scan)
{
	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_PIPE;
	if (scan->f_peek(scan) == '|')
	{
		tok->f_push_back(tok, scan->f_next(scan));
		tok->type = E_TYPE_DOUBLE_PIPE;
	}
}

void	get_double_ampersand(t_token *tok, t_scanner *scan)
{
	char	c1;
	char	c2;

	c1 = scan->f_next(scan);
	c2 = scan->f_next(scan);
	if (c1 == '&' && c2 == '&')
	{
		tok->f_push_back(tok, c1);
		tok->f_push_back(tok, c2);
		tok->type = E_TYPE_DOUBLE_AMPERSAND;
	}
	else
	{
		scan->f_unget(scan);
		scan->f_unget(scan);
		get_cmd_or_arg(tok, scan);
	}
}

void	get_redirection(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_REDIRECT;
	c = scan->f_peek(scan);
	if (c == '<' || c == '>')
		tok->f_push_back(tok, scan->f_next(scan));
}

void	get_double_quote(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (c == '\"')
			break ;
		tok->f_push_back(tok, scan->f_next(scan));
	}
	tok->f_push_back(tok, scan->f_next(scan));
}

void	get_single_quote(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (c == '\'')
			break ;
		tok->f_push_back(tok, scan->f_next(scan));
	}
	tok->f_push_back(tok, scan->f_next(scan));
}
