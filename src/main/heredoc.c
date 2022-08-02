/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:20:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 14:49:24 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <stdio.h>
#include <readline/readline.h>
#include <fcntl.h>
#include <signal.h>

#include "minishell.h"
#include "heredoc.h"
#include "../lexer/token.h"

#define CHILD (0)
#define NO_STOP_OR_EXIT_CHILD (0)

extern int	g_is_sig_interupt;

static void	expand_token(t_token *tok)
{
	t_iterator	iter;
	t_string	*expanded_str;
	char		c;

	init_iterator(&iter, tok->str->text);
	expanded_str = new_string(tok->str->capacity);
	while (iter.f_has_next(&iter))
	{
		c = iter.f_next(&iter);
		if (c == '\'' || c == '\"')
		{
			expand_quote(expanded_str, &iter, c);
			tok->type = E_TYPE_REDIR_ARG_HEREDOC_QUOTED;
		}
		else
			expanded_str->f_push_back(expanded_str, c);
	}
	delete_string(&tok->str);
	tok->str = expanded_str;
}

/* NOTES:
** 0) keep checking pid.
** 1) if non-zero pid is returned, return SUCCESS.
** 2) if zero pid is returned but ^C occcured, return ERROR.
*/
static int	parent_process(int *pipefd, pid_t pid, t_token *tok)
{
	pid_t	ret;

	while (true)
	{
		ret = waitpid(pid, 0, WNOHANG);
		if (ret != NO_STOP_OR_EXIT_CHILD)
		{
			close(pipefd[WRITE]);
			tok->heredoc_fd = pipefd[READ];
			return (SUCCESS);
		}
		else if (g_is_sig_interupt == true)
		{
			g_is_sig_interupt = false;
			kill(pid, SIGTERM);
			return (ERROR);
		}
	}
}

static int	open_heredoc(t_token *tok)
{
	int		pipefd[2];
	pid_t	pid;
	char	*line;

	pipe(pipefd);
	pid = fork();
	if (pid == CHILD)
	{
		close(pipefd[READ]);
		while (true)
		{
			line = readline_prompt_heredoc();
			if (is_limiter(line, tok->str->text) == true)
				exit(EXIT_SUCCESS);
			write(pipefd[WRITE], line, ft_strlen(line));
			write(pipefd[WRITE], "\n", 2);
			free(line);
		}
	}
	else
		return (parent_process(pipefd, pid, tok));
}

int	set_heredoc(t_list *tokens)
{
	t_list	*cur;
	t_token	*tok;

	cur = tokens;
	while (cur)
	{
		tok = cur->content;
		if (tok->type == E_TYPE_REDIR_HEREDOC)
		{
			cur = cur->next;
			tok = cur->content;
			expand_token(tok);
			if (open_heredoc(tok) == ERROR)
				return (ERROR);
		}
		cur = cur->next;
	}
	return (SUCCESS);
}
