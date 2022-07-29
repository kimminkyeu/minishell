/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:23:33 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 16:23:35 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>

#include "../lexer/token.h"

void	expand_quote(t_string *str, t_iterator *iter, char quote_type)
{
	char	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_next(iter);
		if (c == quote_type)
			return ;
		else
			str->f_push_back(str, c);
	}
}

bool	is_limiter(const char *line, const char *limiter)
{
	if (ft_strlen(line) == ft_strlen(limiter)
		&& !ft_strncmp(line, limiter, ft_strlen(limiter)))
		return (true);
	else
		return (false);
}

char	*readline_prompt_heredoc(void)
{
	t_string	*prompt;
	static char	*line;

	prompt = new_string(64);
	prompt->f_append(prompt, "\033[31m");
	prompt->f_append(prompt, "heredoc");
	prompt->f_append(prompt, "\033[0m");
	prompt->f_append(prompt, "$ ");
	line = readline(prompt->text);
	delete_string(&prompt);
	return (line);
}
