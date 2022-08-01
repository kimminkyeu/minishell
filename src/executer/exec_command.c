/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 17:38:17 by han-yeseul        #+#    #+#             */
/*   Updated: 2022/08/01 21:22:53 by minkyeki         ###   ########.fr       */
/*   Updated: 2022/08/01 17:06:47 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "../main/minishell.h"
#include "../parser/parse_tree.h"
#include "../main/signal_handle.h"
#include "executer.h"
#include "exec_command.h"

#define PIPE_ERROR		(1)
#define FORK_ERROR		(-1)
#define CHILD			(0)

#define READ			(0)
#define WRITE			(1)

int	exec_builtin(char **cmd_argv, char ***envp, t_shell_config *config)
{
	size_t	len;
	int		status;

	len = ft_strlen(cmd_argv[0]);
	if (ft_strncmp("cd", cmd_argv[0], len + 1) == 0)
		status = exec_cd(cmd_argv, envp);
	else if (ft_strncmp("exit", cmd_argv[0], len + 1) == 0)
		status = exec_exit(cmd_argv, *envp, config);
	else if (ft_strncmp("export", cmd_argv[0], len + 1) == 0)
		status = exec_export(cmd_argv, envp);
	else if (ft_strncmp("env", cmd_argv[0], len + 1) == 0)
		status = exec_env(cmd_argv, *envp);
	else if (ft_strncmp("echo", cmd_argv[0], len + 1) == 0)
		status = exec_echo(cmd_argv, *envp);
	else if (ft_strncmp("pwd", cmd_argv[0], len + 1) == 0)
		status = exec_pwd(cmd_argv, *envp);
	else if (ft_strncmp("unset", cmd_argv[0], len + 1) == 0)
		status = exec_unset(cmd_argv, envp);
	else
		status = ERROR;
	return (status);
}

/** no fork. */
int	exec_exceptions(t_tree *node, char **cmd_argv, t_shell_config *config)
{
	int	pipe_fd[2];
	int	status;

	pipe_fd[READ] = config->stdin_backup;
	pipe_fd[WRITE] = config->stdout_backup;
	status = open_redirection(pipe_fd, node->redirection, config);
	if (status != SUCCESS)
		return (status);
	if (pipe_fd[READ] != config->stdin_backup)
	{
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);
	}
	if (pipe_fd[WRITE] != config->stdout_backup)
	{
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[WRITE]);
	}
	status = exec_builtin(cmd_argv, config->envp, config);
	if (pipe_fd[READ] != config->stdin_backup)
		dup2(config->stdin_backup, STDIN_FILENO);
	if (pipe_fd[WRITE] != config->stdout_backup)
		dup2(config->stdout_backup, STDOUT_FILENO);
	return (status);
}

int	exec_general(t_tree *node, char **cmd_argv, t_shell_config *config)
{
	int		pipe_fd[2];
	pid_t	pid;

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
	if (pid == CHILD)
		child_process(pipe_fd, node, cmd_argv, config);
	else
		parent_process(pipe_fd, pid, node, config);
	return (SUCCESS);
}

/* exec_priority_operator():
** NOTES:
** 1) CMD_STOP_RUNNING stop running other process.
** STEPS:
** 1) if (), execute subshell.
** 2) if &&, waitpid.
** 3) if ||, waitpid.
*/
void	exec_priority_operator(t_tree *node, t_token *tok, int *status, \
			t_shell_config *config)
{
	if (tok->type == E_TYPE_BRACKET)
		*status = exec_subshell(node, tok->str, config);
	else if (tok->type == E_TYPE_DOUBLE_AMPERSAND)
	{
		wait_every_pid(config);
		if (WEXITSTATUS(config->last_cmd_wstatus) != SUCCESS)
			*status = CMD_STOP_RUNNING;
	}
	else if (tok->type == E_TYPE_DOUBLE_PIPE)
	{
		wait_every_pid(config);
		if (WEXITSTATUS(config->last_cmd_wstatus) == SUCCESS)
			*status = CMD_STOP_RUNNING;
	}
	return ;
}
