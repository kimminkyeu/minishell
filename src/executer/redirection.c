/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 15:34:25 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/23 20:28:37 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

#include <fcntl.h>//open
#include <unistd.h>

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
			write(2, "> ", 2);
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

int	set_redirection(t_pipe *new_io, t_list *redir_list, t_shell_config *shell)// [>] [out] [<] [in]
{
	unsigned char	error_no;
	t_token			*tok;
	t_list			*cur;

	new_io->read = shell->stdin_backup;
	new_io->write = shell->stdout_backup;
	error_no = 0;
	cur = redir_list;
	while (cur != NULL)
	{
		tok = cur->content;
		if (tok->type == E_TYPE_REDIR_LESS)
		{
			cur = cur->next;
			tok = cur->content;
			new_io->read = open(tok->str->text, O_RDONLY);
			if (new_io->read == -1)
				break ;
		}
		else if (tok->type == E_TYPE_REDIR_HEREDOC)
		{
			cur = cur->next;
			tok = cur->content;
			open_heredoc(tok->str->text);
		}
		else if (tok->type == E_TYPE_REDIR_GREATER)
		{
			new_io->write = open(tok->str->text, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (new_io->write == -1)
				break ;
		}
		else if (tok->type == E_TYPE_REDIR_APPEND)
		{
			new_io->write = open(tok->str->text, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (new_io->write == -1)
				break ;
		}
		cur = cur->next;
	}
	if (cur == NULL)
	{
		//break로 나오면 open()이 실패해서 errno이 설정되었을 테고,
		//여기서 중단되고 에러메시지가 뜹니다.
		//다만, 뒤에 heredoc이 남아있을 경우에 모든 heredoc에 대한 입력을 받습니다.
		error_no = errno;
		while (cur != NULL)
		{
			tok = cur->content;
			if (tok->type == E_TYPE_REDIR_HEREDOC)
			{
				cur = cur->next;
				tok = cur->content;
				open_heredoc(tok->str->text);
			}
		}
	}
	return (error_no);
	//exit() 해야 한다면 밖에서
}

