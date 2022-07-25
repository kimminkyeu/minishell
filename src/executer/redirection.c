/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 15:34:25 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 22:59:27 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

#include <fcntl.h>//open
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>//strerror

#include "executer.h"
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

void	set_pipe(int *my_io, int *pipefd, t_shell_config *config, t_tree *node)
{
	if (config->pipefd_save != -1)
		my_io[READ] = config->pipefd_save;
	if (node->is_last_pipe_cmd == false)
		my_io[WRITE] = pipefd[WRITE];

}

int	open_redirection(int *my_io, t_list *redir_list)// [>] [out] [<] [in]
{
	int				status;
	t_token			*tok;
	t_list			*cur;

	status = 0;
	if (redir_list != NULL)
	{
		cur = redir_list;
		while (cur != NULL)
		{
			tok = cur->content;
			if (tok->type == E_TYPE_REDIR_LESS) // < in
			{
				cur = cur->next;
				tok = cur->content;
				my_io[READ] = open(tok->str->text, O_RDONLY);
				if (my_io[READ] == -1)
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
				my_io[WRITE] = open(tok->str->text, O_WRONLY | O_TRUNC | O_CREAT, 0644);
				if (my_io[WRITE] == -1)
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
				my_io[WRITE] = open(tok->str->text, O_WRONLY | O_APPEND | O_CREAT, 0644);
				if (my_io[WRITE] == -1)
				{
					status = errno;
				printf("lesh: %s: %s\n", tok->str->text, strerror(status));
					break ;
				}
			}
			cur = cur->next;
		}
	}

	return (status);
}

void	set_redirection(int *my_io)
{
	if (dup2(my_io[READ], STDIN_FILENO) == -1) //파이프가 닫혔나?
		perror("dup2 read");
	if (dup2(my_io[WRITE], STDOUT_FILENO) == -1)
		perror("dup2 write");
}
