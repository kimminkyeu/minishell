/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 15:34:25 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/01 15:43:54 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

/** Readline library */
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <fcntl.h>//open
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>//strerror

#include "../main/minishell.h"
#include "../lexer/token.h"
#include "redirection.h"

#define IN		(0)
#define OUT		(1)
#define CHILD	(0)

#define READ	(0)
#define WRITE	(1)

int	open_file_less(t_list *cur, int *pipe_fd, int *status)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;
	pipe_fd[READ] = open(tok->str->text, O_RDONLY);
	if (pipe_fd[READ] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

/* open_file_heredoc():
** STEPS:
** 1) if limiter has never been moved,
** replace tok->heredoc_fd with a new file with expanded contents.
** 2) set pipe_fd[READ] with heredoc_fd
*/
int	open_file_heredoc(t_list *cur, int *pipe_fd, int *status, \
		t_shell_config *config)
{
	t_token	*tok;

	(void)config;
	cur = cur->next;
	tok = cur->content;
	if (tok->type != E_TYPE_REDIR_ARG_HEREDOC_QUOTED)
		expand_file(tok, config);
	pipe_fd[READ] = tok->heredoc_fd;
	if (pipe_fd[READ] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

int	open_file_greater(t_list *cur, int *pipe_fd, int *status)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;
	pipe_fd[WRITE] = open(tok->str->text, O_WRONLY | O_TRUNC | O_CREAT, 0600);
	if (pipe_fd[WRITE] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

int	open_file_append(t_list *cur, int *pipe_fd, int *status)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;
	pipe_fd[WRITE] = open(tok->str->text, O_WRONLY | O_APPEND | O_CREAT, 0600);
	if (pipe_fd[WRITE] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

int	open_redirection(int *pipe_fd, t_list *redir_list, \
			t_shell_config *config)
{
	int				status;
	t_token			*tok;
	t_list			*cur;

	status = SUCCESS;
	if (redir_list != NULL)
	{
		cur = redir_list;
		while (cur != NULL && status == SUCCESS)
		{
			tok = cur->content;
			if (tok->type == E_TYPE_REDIR_LESS)
				open_file_less(cur, pipe_fd, &status);
			else if (tok->type == E_TYPE_REDIR_HEREDOC)
				open_file_heredoc(cur, pipe_fd, &status, config);
			else if (tok->type == E_TYPE_REDIR_GREATER)
				open_file_greater(cur, pipe_fd, &status);
			else if (tok->type == E_TYPE_REDIR_APPEND)
				open_file_append(cur, pipe_fd, &status);
			cur = cur->next;
		}
	}
	return (status);
}
