/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 20:21:34 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 19:17:32 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "executer.h"
#include "../builtin/environ.h"
#include "../main/minishell.h"
#include "../lexer/token.h"
#include "../main/signal_handle.h"

#define PIPE_ERROR		(1)
#define FORK_ERROR		(-1)
#define CHILD			(0)

#define READ			(0)
#define WRITE			(1)

#define EXIT_COMMAND_NOT_FOUND (127)

static int	pipe_and_fork(int *pipe_fd, pid_t *pid)
{
	if (pipe(pipe_fd) == PIPE_ERROR)
	{
		perror("pipe()");
		return (ERROR);
	}
	*pid = fork();
	if (*pid == FORK_ERROR)
	{
		perror("fork()");
		return (ERROR);
	}
	return (SUCCESS);
}

static void	child_process(int *pipe_fd, t_tree *node, t_shell_config *config)
{
	int	tmp_fd;

	if (node->is_last_pipe_cmd)
		dup2(config->stdout_backup, pipe_fd[WRITE]);
	tmp_fd = pipe_fd[READ];
	open_redirection(pipe_fd, node->redirection, config);
	if (tmp_fd != pipe_fd[READ])
		dup2(pipe_fd[READ], STDIN_FILENO);
	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[WRITE]);
	close(pipe_fd[READ]);
}

static void	parent_process(int *pipe_fd, t_tree *node, \
				t_shell_config *config, pid_t pid)
{
	ft_lstadd_back(&config->pid_list, ft_lstnew(new_pid(pid)));
	if (node->is_last_pipe_cmd)
	{
		config->last_cmd_pid = pid;
		dup2(config->stdin_backup, STDIN_FILENO);
		dup2(config->stdout_backup, STDOUT_FILENO);
	}
	if (!node->is_last_pipe_cmd)
		dup2(pipe_fd[READ], STDIN_FILENO);
	close(pipe_fd[WRITE]);
	close(pipe_fd[READ]);
}

int	exec_subshell(t_tree *node, t_string *str, t_shell_config *config)
{
	t_shell_config	new_config;
	pid_t			pid;
	int				pipe_fd[2];
	char			*line; // 입력 라인.

	/** (1) set_up new_config */
	load_shell_config(&new_config, *(config->envp));
	/** (2) delete bracket ( ) */
	line = ft_substr(str->text, 1, ft_strlen(str->text) - 2);
	/** (3) fork and run Subshell */
	if (pipe_and_fork(pipe_fd, &pid) == ERROR)
		return (ERROR);
	if (pid == CHILD) /* child */
	{
		child_process(pipe_fd, node, config);
		/** SubShell 의 실행결과를 exit으로 전달.  */
		exit(run_shell(line, &new_config));
	}
	else /* parent */
	{
		parent_process(pipe_fd, node, config, pid);
		free(line);
		return (SUCCESS);
	}
	/** FIXME : 여기서 free해주면 자식에게 영향이 가지 않을까? --> 문제 가능성 (1)
	 * -> 자식은 다른 메모리를 쓰므로 부모프로세스에 넣어도 될 것 같아서 우선 넣었습니다! */
}
