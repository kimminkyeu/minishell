/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 15:34:25 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 13:59:34 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>//open
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>//strerror
				   //
/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

#include "../main/minishell.h"
#include "../lexer/token.h"

# define IN		(0)
# define OUT	(1)
# define CHILD	(0)

# define READ	(0)
# define WRITE	(1)

int	is_limiter(char *line, const char *limiter)
{
	/** FIXME : 줄바꿈이 있을 수도 있지 않나? 이부분 어쩌지?  */
	if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		return (false);
	else
		return (true);
}

int	open_heredoc(const char *limiter)
{
	// 여기는 pipex heredoc 코드에서 readline 부분만 바꾼 상태예요.
	// heredoc 관련 내용:
	// 노션 여기서 봐주세요.
	// (https://www.notion.so/kyeu/Minishell-110e3b17e97e467eb44e1ef23b9e8882#df7f3307d1ce4456a6d3fbb7ca11e7b7)
	int			pipe_fd[2];
	pid_t		pid;
	char		*line;

	line = NULL;
	pipe(pipe_fd);
	pid = fork();
	if (pid == CHILD)
	{
		close(pipe_fd[READ]);
		while (true)
		{
			write(2, "& ", 2);
			line = readline("\033[31mheredoc> \033[0m");
			if (is_limiter(line, limiter))
			{
				free(line);
				exit(EXIT_SUCCESS);
			}
			else
				write(pipe_fd[WRITE], line, ft_strlen(line));
		}
	}
	free(line);
	close(pipe_fd[WRITE]);
	wait(NULL);
	return (pipe_fd[READ]);
}

int	open_redirection(int *pipe_fd, t_list *redir_list /*, t_shell_config *config*/)// [>] [out] [<] [in]
{
	int				status;
	t_token			*tok;
	t_list			*cur;

	/** config->pipe_fd[READ] == config->stdin_backup; */
	/** config->pipe_fd[WRITE] == config->stdout_backup; */

	status = 0;
	if (redir_list == NULL)
		return (status);
	cur = redir_list;
	while (cur != NULL)
	{
		tok = cur->content;
		if (tok->type == E_TYPE_REDIR_LESS) // < in
		{
			cur = cur->next;
			tok = cur->content;
			pipe_fd[READ] = open(tok->str->text, O_RDONLY);
			if (pipe_fd[READ] == -1)
			{
				status = errno;
				printf("lesh: %s: %s\n", tok->str->text, strerror(status));
				break ;
			}
		}
		else if (tok->type == E_TYPE_REDIR_GREATER) // > out
		{
			cur = cur->next;
			tok = cur->content;
			pipe_fd[WRITE] = open(tok->str->text, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (pipe_fd[WRITE] == -1)
			{
				status = errno;
				printf("lesh: %s: %s\n", tok->str->text, strerror(status));
				break ;
			}
		}
		else if (tok->type == E_TYPE_REDIR_APPEND) // >> out
		{
			cur = cur->next;
			tok = cur->content;
			pipe_fd[WRITE] = open(tok->str->text, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (pipe_fd[WRITE] == -1)
			{
				status = errno;
				printf("lesh: %s: %s\n", tok->str->text, strerror(status));
				break ;
			}
		}
		cur = cur->next;
	}

	return (status);
}
