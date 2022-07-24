/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 00:06:10 by minkyeki         ###   ########.fr       */
/*   Updated: 2022/07/23 14:33:08 by minkyeki         ###   ########.fr       */
/*   Updated: 2022/07/22 13:20:52 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include <sys/wait.h>
#include "executer.h"
#include "../../include/builtin.h"
#include "../main/minishell.h"
#include "../lexer/token.h"

# define PIPE_ERROR		(1)
# define FORK_ERROR		(-1)
# define CHILD			(0)

# define EXIT_COMMAND_NOT_FOUND (127)

void	delete_tree_node(t_tree *node, int *status, t_shell_config *config)
{
	(void)status;
	(void)config;
	if (node != NULL)
	{
		if ((node)->redirection != NULL)
			ft_lstclear(&node->redirection, delete_token);
		if ((node)->token != NULL)
			ft_lstclear(&node->token, delete_token);
		free(node);

		/** printf("\033[90mnode deleted\033[0m\n"); */
	}
}

int	is_cd_or_exit_or_export(char *cmd)
{
	// 뒤에서 부터 검사 진행 ---> 경로가 포함될 수도 있기 때문.
	size_t	len;

	len = ft_strlen(cmd);
	if (ft_strncmp("cd", cmd, len + 1) != 0 \
			&& ft_strncmp("exit", cmd, len + 1) != 0 \
			&& ft_strncmp("export", cmd, len + 1) != 0)
		return (false);
	else
		return (true);
}

/** TODO : 이 부분 로직 좀 더 효율적인 방법 없나? */
int	is_builtin_func(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (ft_strncmp("cd", cmd, len + 1) != 0\
			&& ft_strncmp("exit", cmd, len + 1) != 0 \
			&& ft_strncmp("export", cmd, len + 1) != 0\
			&& ft_strncmp("env", cmd, len + 1) != 0 \
			&& ft_strncmp("echo", cmd, len + 1) != 0 \
			&& ft_strncmp("pwd", cmd, len + 1) != 0 \
			&& ft_strncmp("unset", cmd, len + 1) != 0)
		return (false);
	else
		return (true);
}

/** TODO : 이 부분 로직 좀 더 효율적인 방법 없나? */
int	exec_builtin(char **cmd_argv, char ***envp)
{
	size_t	len;
	int		status;

	/** printf("here\n"); */

	len = ft_strlen(cmd_argv[0]);
	if (ft_strncmp("cd", cmd_argv[0], len + 1) == 0)
		status = exec_cd(cmd_argv, envp);
	else if (ft_strncmp("exit", cmd_argv[0], len + 1) == 0)
		status = exec_exit(cmd_argv, *envp);
	else if (ft_strncmp("export", cmd_argv[0], len + 1) == 0)
		status = exec_export(cmd_argv, envp);
	else if(ft_strncmp("env", cmd_argv[0], len + 1) == 0)
		status = exec_env(cmd_argv, *envp);
	else if(ft_strncmp("echo", cmd_argv[0], len + 1) == 0)
		status = exec_echo(cmd_argv, *envp);
	else if(ft_strncmp("pwd", cmd_argv[0], len + 1) == 0)
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
	t_pipe	pipe_fd2;
	int		status;

	printf("\033[90mcalling exec_exceptions() : %s\033[0m\n\n", cmd_argv[0]);

	set_redirection(&pipe_fd2, node->redirection, config);
	status = exec_builtin(cmd_argv, config->envp);
	return (status);
}

/** TODO : Redirection 복구는 last_pipe_cmd에서 해줘야 한다. */
int	exec_general(t_tree *node, char **cmd_argv, t_shell_config *config) // 무조건 fork를 하는 애들.
{
	pid_t	pid;
	t_pipe	pipe_fd2; // == fd[2]

	char	*full_path;

	printf("\033[90mcalling exec_general() : %s\033[0m\n\n", cmd_argv[0]);

	if (pipe(pipe_fd2.data) == PIPE_ERROR)
	{
		perror("pipe()");
		return (ERROR);// pipe function error...
	}

	set_redirection(&pipe_fd2, node->redirection, config);

	pid = fork();
	if (pid == FORK_ERROR)
		return (ERROR);// fork() function error...

	if (pid != CHILD) // if parent
	{
		if (node->is_last_pipe_cmd)
			config->last_cmd_pid = pid; // save last_cmd's pid
		else // 마지막 커맨드가 아니면 STDIN을 바꿔주면 됨.
		{
			/** printf("changing redir out\n"); */

			dup2(pipe_fd2.read, STDIN_FILENO);
			close(pipe_fd2.read);
			close(pipe_fd2.write);
		}
	}
	else if (pid == CHILD) // if parent
	{
		full_path = NULL;
		dup2(pipe_fd2.write, STDOUT_FILENO);
		close(pipe_fd2.read);
		close(pipe_fd2.write);

		if (is_builtin_func(cmd_argv[0]) == true)
		{
			int status = exec_builtin(cmd_argv, config->envp);
			/** NOTE : 빌트인에서도 실행 후 exit을 해줘야 한다...
			 * 그게 오류의 원인이였다... */
			exit(status);
		}
		if (cmd_argv[0] != NULL && ft_strchr(cmd_argv[0], '/') == NULL)
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
	return (SUCCESS);
}

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	if (*status != CMD_SUCCEESS || node == NULL)
		return ;

	t_token *tok = node->token->content;

	/** (1) if | or && or || or ( ), do not expand tokens. */
	if (tok->type != E_TYPE_SIMPLE_CMD)
	{
		/* handle_operation(t_shell_config *config)  */
		/** (1-1) if [() : subshell] */
		/** (1-2) if [| : pipe, do nothing] */
		/** (1-3) if [&& : waitpid] */
		/** (1-4) if [|| : waitpid] */
		return ;
	}

	/** (2) if simple command, expand tokens */
	if (expand_tokens(node->token, config) == ERROR \
			|| expand_tokens(node->redirection, config) == ERROR)
		return ;

	/** (3) make token to char **cmd_argv */
	char	**cmd_argv = get_cmd_argv(node->token);  /** print_strs(cmd_argv); */

	/** NOTE: 수정 이후.  */
	if (node->is_pipeline == false && is_builtin_func(cmd_argv[0])) // 무조건 fork없이 실행.
		*status = exec_exceptions(node, cmd_argv, config);
	else
		*status = exec_general(node, cmd_argv, config); //  무조건 fork를 하는 애들.

	/** NOTE: 수정 이전. unset이 fork가 먹어서 안돌아감.  */
/**     if (!(node->is_pipeline) && is_cd_or_exit_or_export(cmd_argv[0]))
  *         *status = exec_exceptions(node, cmd_argv, config); // fork없이 실행
  *     else
  *         *status = exec_general(node, cmd_argv, config); //  무조건 fork를 하는 애들.
  *  */


	dup2(config->stdin_backup, STDIN_FILENO);
	dup2(config->stdout_backup, STDOUT_FILENO);
	delete_strs(&cmd_argv);

	/** NOTE : if success, then set status to ... CMD_SUCCESS
	 *         else, set status to ...            CMD_FAILURE
	 *         if exit, then set status to ...    CMD_STOP_SHELL
	 * */
	/** *status = CMD_SUCCEESS; */
	/** *status = CMD_STOP_SHELL; */
	/** *status = CMD_FAILURE; */
}

/** 함수 포인터 글자수 줄이는 용도 */
typedef void(*t_callback_func)(t_tree *, int *, t_shell_config *);

void	inorder_recur(t_tree *node, int *status, t_callback_func callback, t_shell_config *shell_config)
{
	/** status가 몇일 때 어떤 행동을 할지는 구현할 때 정하기 */
	if (node == NULL)
		return ;

	/** (1) if status == CMD_STOP_SHELL (ex. calling exit) then stop all */

	/** NOTE : 노드 삭제 함수의 경우 에러 여부와 관계 없이 순회해야함.  */
	if (callback != delete_tree_node && *status == CMD_SUCCEESS)
	{
		inorder_recur(node->left, status, callback, shell_config);
		callback(node, status, shell_config);
		inorder_recur(node->right, status, callback, shell_config);
	}
	else if (callback == delete_tree_node)
	{
		inorder_recur(node->left, status, callback, shell_config);
		inorder_recur(node->right, status, callback, shell_config);
		callback(node, status, shell_config);
	}
}

/** Executer function. make traversing */
int	execute(t_tree *syntax_tree, t_shell_config *config)
{
	int	status;
	int	wait_status;

	wait_status = 0xffffffff;
	status = CMD_SUCCEESS;

	printf("\n");
	printf("------------------------------------------\n");
	printf("|     Execution (inorder recursive)      |\n");
	printf("------------------------------------------\n\n");

	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node, config);

	// 여기서 마지막 pid_t를 기다린다.
	waitpid(config->last_cmd_pid, &wait_status, 0);
	printf("\n");
	printf("\033[90mexecute() : waiting pid %d\033[0m\n", config->last_cmd_pid);
	printf("\033[90mexecute() : child's exit code = %d\033[0m\n", WEXITSTATUS(wait_status));
	printf("\n\n\n");

	/** 모든 노드 삭제 */
	inorder_recur(syntax_tree, &status, delete_tree_node, config);

	return (status);
}
