/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:06:35 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 18:30:52 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

t_list	*tokenize(char *line)
{
	t_list		*token_list;

	token_list = create_initial_tokens(line);
	if (is_syntax_error(token_list))
	{
		ft_lstclear(&token_list, delete_token);
		return (NULL);
	}
	set_redirection_type(token_list);
	return (token_list);
}

int	is_syntax_error(t_list *token_list)
{
	if (is_meta_token_overlap(token_list))
		return (true);
	else if (is_last_token_meta(token_list))
		return (true);
	return (false);
}

t_list	*create_initial_tokens(char *line)
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
		else if (c == '<' || c == '>')
			get_redirection(token, &scanner);
		else if (c == '(')
			get_bracket(token, &scanner);
		else
			get_cmd_or_arg(token, &scanner);
		ft_lstadd_back(&token_list, ft_lstnew(token));
	}
	return (token_list);
}

void	set_redirection_type(t_list *token_list)
{
	t_list	*tmp;
	t_token	*tok;

	tmp = token_list;
	while (tmp->next != NULL)
	{
		tok = tmp->content;
		if (tok->type == E_TYPE_REDIRECT)
		{
			if (ft_strncmp(tok->str->text, "<", 2) == 0)
				tok->type = E_TYPE_REDIR_LESS;
			else if (ft_strncmp(tok->str->text, ">", 2) == 0)
				tok->type = E_TYPE_REDIR_GREATER;
			else if (ft_strncmp(tok->str->text, "<<", 3) == 0)
				tok->type = E_TYPE_REDIR_HEREDOC;
			else if (ft_strncmp(tok->str->text, ">>", 3) == 0)
				tok->type = E_TYPE_REDIR_APPEND;
			tmp = tmp->next;
			tok = tmp->content;
			tok->type = E_TYPE_REDIR_ARG;
			continue ;
		}
		tmp = tmp->next;
	}
}