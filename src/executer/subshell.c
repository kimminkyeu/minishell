/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 20:21:34 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 03:32:18 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "executer.h"
#include "../builtin/environ.h"
#include "../main/minishell.h"
#include "../lexer/token.h"
#include "../main/signal_handle.h"

# define PIPE_ERROR		(1)
# define FORK_ERROR		(-1)
# define CHILD			(0)

# define READ			(0)
# define WRITE			(1)

# define EXIT_COMMAND_NOT_FOUND (127)

int	exec_subshell(t_tree *node, t_string *str, t_shell_config *config)
{
	t_shell_config	new_config;
	pid_t			pid;
	int				pipe_fd[2];
	int				tmp_fd;
	char			*line; // 입력 라인.

	/** (1) set_up new_config */
	load_shell_config(&new_config, *(config->envp));

	/** (2) delete bracket ( ) */
	line = ft_substr(str->text, 1, ft_strlen(str->text) - 2);

	/** (3) fork and run Subshell */
	if (pipe(pipe_fd) == PIPE_ERROR)
	{
		perror("pipe()");
		return (ERROR);
	}
	pid = fork();
	if (pid == FORK_ERROR)
	{
		perror("fork()");
		return (ERROR);
	}	
	if (pid == CHILD) /* child */
	{
		if (node->is_last_pipe_cmd)
			dup2(config->stdout_backup, pipe_fd[WRITE]);
		tmp_fd = pipe_fd[READ];
		open_redirection(pipe_fd, node->redirection);
		if (tmp_fd != pipe_fd[READ])
			dup2(pipe_fd[READ], STDIN_FILENO);
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[WRITE]);
		close(pipe_fd[READ]);
		/** SubShell 의 실행결과를 exit으로 전달.  */
		exit(run_shell(line, &new_config));
	}
	else /* parent */
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
	/** FIXME : 여기서 free해주면 자식에게 영향이 가지 않을까? --> 문제 가능성 (1) */
	free(line);
	return (SUCCESS);
}

