/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 15:34:25 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 17:08:06 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Readline library */
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

int	open_file_heredoc(t_list *cur, int *pipe_fd, int *status, \
		t_shell_config *config)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;

	//1. limiter가 쿼트리무벌된 적이 없으면, tok->heredoc_fd를 내용이 확장된 새로운 파일fd로 교체한다.
	if (tok->type != E_TYPE_REDIR_ARG_HEREDOC_QUOTED)
		expand_file(tok, config);

	//2. fd를 넣어준다.
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

void	open_redirection(int *pipe_fd, t_list *redir_list, \
			t_shell_config *config)
{
	int		status;
	t_token	*tok;
	t_list	*cur;

	if (redir_list == NULL)
		return ;
	status = 0;
	cur = redir_list;
	while (cur != NULL)
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

		if (status != SUCCESS)
			exit(status);
		else
			cur = cur->next;
	}
}
