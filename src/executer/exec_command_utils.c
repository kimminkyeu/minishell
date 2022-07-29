/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 17:35:53 by han-yeseul        #+#    #+#             */
/*   Updated: 2022/07/29 16:25:07 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../main/minishell.h"
#include "../parser/parse_tree.h"
#include "../main/signal_handle.h"
#include "exec_command.h"
#include "executer.h"

#define EXIT_COMMAND_NOT_FOUND (127)

int	is_cd_or_exit_or_export(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (ft_strncmp("cd", cmd, len + 1) != 0 \
			&& ft_strncmp("exit", cmd, len + 1) != 0 \
			&& ft_strncmp("export", cmd, len + 1) != 0)
		return (false);
	else
		return (true);
}

int	is_builtin_func(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (ft_strncmp("cd", cmd, len + 1) != 0 \
			&& ft_strncmp("exit", cmd, len + 1) != 0 \
			&& ft_strncmp("export", cmd, len + 1) != 0 \
			&& ft_strncmp("env", cmd, len + 1) != 0 \
			&& ft_strncmp("echo", cmd, len + 1) != 0 \
			&& ft_strncmp("pwd", cmd, len + 1) != 0 \
			&& ft_strncmp("unset", cmd, len + 1) != 0)
		return (false);
	else
		return (true);
}

/** exec_commmand():
 ** NOTES:
 ** 1) this is sub-function for exec_general().
 **/
void	exec_command(char **cmd_argv, t_shell_config *config)
{
	char	*full_path;

	full_path = NULL;
	if (cmd_argv == NULL)
		exit(SUCCESS);
	if (is_builtin_func(cmd_argv[0]) == true)
	{
		exit(exec_builtin(cmd_argv, config->envp));
	}
	if (cmd_argv[0] != NULL)
		full_path = get_full_path(cmd_argv[0], *config->envp);
	if (full_path != NULL)
	{
		free(cmd_argv[0]);
		cmd_argv[0] = full_path;
		execve(cmd_argv[0], cmd_argv, *config->envp);
	}
	else
	{
		ft_putstr_fd(cmd_argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
}

/** child_process():
 ** NOTES:
 ** 1) this is sub-function for exec_general().
 ** STEPS:
 ** 1) if the only or last command, write to STDOUT.
 ** 2) update IN, only if open_redirection() changes pipe_fd[READ].
 ** 3) update OUT with pipe_fd[WRITE].
 ** 4) execute command with 1) ~ 3).
 **/
void	child_process(int *pipe_fd, t_tree *node, char **cmd_argv, \
			t_shell_config *config)
{
	int		tmp_fd;

	if (node->is_last_pipe_cmd)
		dup2(config->stdout_backup, pipe_fd[WRITE]);
	tmp_fd = pipe_fd[READ];
	open_redirection(pipe_fd, node->redirection, config);
	if (tmp_fd != pipe_fd[READ])
		dup2(pipe_fd[READ], STDIN_FILENO);
	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[WRITE]);
	close(pipe_fd[READ]);
	exec_command(cmd_argv, config);
}

/** parent_process():
 ** NOTES:
 ** 1) this is sub-function for exec_general().
 ** STEPS:
 ** 1) create pid_list for keep each pid(for waitpid() later).
 ** 2) if the only or last command, restore STDIN, STDOUT
 ** 3) if you are middle of the pipeline, update STDIN for next pipe command.
 **/
void	parent_process(int *pipe_fd, pid_t pid, t_tree *node, \
			t_shell_config *config)
{
	ft_lstadd_back(&config->pid_list, ft_lstnew(new_pid(pid)));
	if (node->is_last_pipe_cmd)
	{
		config->last_cmd_pid = pid;
		dup2(config->stdin_backup, STDIN_FILENO);
		dup2(config->stdout_backup, STDOUT_FILENO);
	}
	if (!node->is_last_pipe_cmd)
	{
		dup2(pipe_fd[READ], STDIN_FILENO);
	}
	close(pipe_fd[WRITE]);
	close(pipe_fd[READ]);
}
