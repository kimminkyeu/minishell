/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 16:03:41 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 22:18:34 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "iterator.h"
#include "token.h"
#include "scanner.h"

/** TODO : move below functions to scanner_utils.c/h */
void	init_scanner(t_scanner *scan, char *line)
{
	init_iterator(&scan->iter, line);
	scan->f_has_next = scanner_has_next;
	scan->f_next = scanner_next;
	scan->f_unget = scanner_unget;
	scan->f_peek = scanner_peek;
	scan->f_skip_white_space = scanner_skip_white_space;
}

int		scanner_has_next(t_scanner *scan)
{
	return (scan->iter.f_has_next(&(scan->iter)));
}

char	scanner_next(t_scanner *scan)
{
	return (scan->iter.f_next(&(scan->iter)));
}

void	scanner_unget(t_scanner *scan)
{
	scan->iter.f_unget(&(scan->iter));
}

char	scanner_peek(t_scanner *scan)
{
	return (scan->iter.f_peek(&(scan->iter)));
}

void	scanner_skip_white_space(t_scanner *scan)
{
	scan->iter.f_skip_white_space(&(scan->iter));
}

/** --------------------------------------------------------------- */

bool	is_meta_char(char c)
{
	if (c != '|' && c != '&' && c != '<' && c != '>' \
			&& /*c != '(' && c != ')' &&*/ c != ';' \
			/* && c != '\'' && c != '\"' */ )
		return (false);
	else
		return (true);

}

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
	/** add to token, only if - double ampersand */
	char	c1;
	char	c2;

	c1 = scan->f_next(scan);
	c2 = scan->f_next(scan);
	if (c1 == '&' && c2 == '&')
	{
		tok->f_push_back(tok, c1);
		tok->f_push_back(tok, c2);
		tok->type = E_TYPE_AMPERSAND;
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
	tok->type = E_TYPE_DOUBLE_QUOTE;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (ft_isspace(c))
		{
			tok->f_push_back(tok, scan->f_next(scan));
			scan->f_skip_white_space(scan);
		}
		else if (is_meta_char(c))
			break ;
		else
			tok->f_push_back(tok, scan->f_next(scan));
	}
}

void	get_single_quote(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_SINGLE_QUOTE;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (ft_isspace(c))
		{
			tok->f_push_back(tok, scan->f_next(scan));
			scan->f_skip_white_space(scan);
		}
		else if (is_meta_char(c))
			break ;
		else
			tok->f_push_back(tok, scan->f_next(scan));
	}
}

void	get_semicolon(t_token *tok, t_scanner *scan)
{
	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_SEMICOLON;
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

void	get_cmd_option(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_CMD_OPTION;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (!ft_isspace(c) && !is_meta_char(c) && c != '\'' && c != '\"' \
				&& c != '(' && c != ')')
			tok->f_push_back(tok, scan->f_next(scan));
		else
			break ;
	}

}

void	get_cmd_or_arg(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_CMD_OR_ARG;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (!ft_isspace(c) \
				&& c != '|'  /* && c != '&' */ && c != '<' && c != '>' \
				&& c != '(' && c != ')' && c != ';' \
				&& c != '\'' && c != '\"')
			tok->f_push_back(tok, scan->f_next(scan));
		else
			break ;
	}
}

t_list	*tokenize(char *line)
{
	t_scanner	scanner;
	t_list		*token_list;
	t_token		*token;
	char		c;

	token_list = NULL;
	init_scanner(&scanner, line);
	while (scanner.f_has_next(&scanner))
	{
		if (ft_isspace(scanner.f_peek(&scanner)))
		{
			scanner.f_skip_white_space(&scanner);
			continue ;
		}
		c = scanner.f_peek(&scanner);
		token = new_token("");
		if (c == '|')
			get_pipe(token, &scanner);
		else if (c == '&')
			get_double_ampersand(token, &scanner);
		else if (c == ';')
			get_semicolon(token, &scanner);
		else if (c == '<' || c == '>')
			get_redirection(token, &scanner);
		else if (c == '\"')
			get_double_quote(token, &scanner);
		else if (c == '\'')
			get_single_quote(token, &scanner);
		else if (c == '(')
			get_bracket(token, &scanner);
		else if (c == '-')
			get_cmd_option(token, &scanner);
		else
			get_cmd_or_arg(token, &scanner);
		ft_lstadd_back(&token_list, ft_lstnew(token));
	}
	return (token_list);
}
