/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/23 22:31:53 by minkyeki         ###   ########.fr       */
/*   Updated: 2022/07/23 14:33:08 by minkyeki         ###   ########.fr       */
/*   Updated: 2022/07/22 13:20:52 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include "executer.h"
#include "../../include/builtin.h"
#include "../main/minishell.h"
#include "../lexer/token.h"

# define PIPE_ERROR		(1)
# define FORK_ERROR		(-1)
# define CHILD			(0)

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

		printf("\033[90mnode deleted\033[0m\n");
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
int	exec_builtin(char **cmd_argv, char **envp)
{
	size_t	len;
	int		status;

	printf("EXECUTE_BUILITIN() : %s\n", cmd_argv[0]);

	len = ft_strlen(cmd_argv[0]);
	if (ft_strncmp("cd", cmd_argv[0], len + 1) == 0)
		status = exec_cd(cmd_argv, envp);
	else if (ft_strncmp("exit", cmd_argv[0], len + 1) == 0)
		status = exec_exit(cmd_argv, envp);
	else if (ft_strncmp("export", cmd_argv[0], len + 1) == 0)
		status = exec_export(cmd_argv, &envp);
	else if(ft_strncmp("env", cmd_argv[0], len + 1) == 0)
		status = exec_env(cmd_argv, envp);
	else if(ft_strncmp("echo", cmd_argv[0], len + 1) == 0)
		status = exec_echo(cmd_argv, envp);
	else if(ft_strncmp("pwd", cmd_argv[0], len + 1) == 0)
		status = exec_pwd(cmd_argv, envp);
	else
		status = exec_unset(cmd_argv, &envp);
	return (status);
}

/** no fork. */
int	exec_exceptions(t_tree *node, char **cmd_argv, t_shell_config *config)
{
	t_pipe	pipe_fd2;
	int		status;

	printf("EXECUTE_EXEPTION() : %s\n", cmd_argv[0]);

	set_redirection(&pipe_fd2, node->redirection, config);
	status = exec_builtin(cmd_argv, config->envp);
	return (status);
}

/** TODO : Redirection 복구는 last_pipe_cmd에서 해줘야 한다. */
int	exec_general(t_tree *node, char **cmd_argv, t_shell_config *config) // 무조건 fork를 하는 애들.
{
	pid_t	pid;
	t_pipe	pipe_fd2; // == fd[2]


	printf("EXECUTE_GENERAL() : %s\n", cmd_argv[0]);

	/** if not last pipe cmd, run pipe() function. #PERROR is 1 */
	if (pipe(pipe_fd2.data) == PIPE_ERROR)
		return (ERROR);// pipe function error...

	/** pipe_fd2에 set_redirection을 덮어쓰면, Stdin Stdout이 자동으로 세팅.  */

	/** FIXME : 여기서 에러가 발생함 ! */
	set_redirection(&pipe_fd2, node->redirection, config);

	/** printf("Check 1 : %s\n", cmd_argv[0]); */

	pid = fork();
	if (pid == FORK_ERROR)
		return (ERROR);// fork() function error...

	if (pid != CHILD) // if parent
	{
		/** printf("Check 2 : %s\n", cmd_argv[0]); */
		if (node->is_last_pipe_cmd)
			config->last_cmd_pid = pid; // save last_cmd's pid
		dup2(pipe_fd2.read, STDIN_FILENO);
		close(pipe_fd2.read);
		close(pipe_fd2.write);
	}
	else if (pid == CHILD) // if parent
	{
		/** printf("Check 3 : %s\n", cmd_argv[0]); */

		dup2(pipe_fd2.write, STDOUT_FILENO);
		/** printf("Check 3-1 : %s\n", cmd_argv[0]); */
		close(pipe_fd2.read);
		/** printf("Check 3-2 : %s\n", cmd_argv[0]); */
		close(pipe_fd2.write);

		/** printf("Check 4 : %s\n", cmd_argv[0]); */
		if (is_builtin_func(cmd_argv[0]) == true)
		{
			return (exec_builtin(cmd_argv, config->envp));
		}

		/** printf("Check 5 : %s\n", cmd_argv[0]); */
		if (cmd_argv[0] != NULL && ft_strchr(cmd_argv[0], '/') == NULL)
		{
			char *tmp = get_full_path(cmd_argv[0], config->envp);
			free(cmd_argv[0]);
			cmd_argv[0] = tmp;
		}

		/** printf("cmd: %s\n", cmd_argv[0]); */

		if (cmd_argv[0] == NULL)
			;//error
	 	execve(cmd_argv[0], cmd_argv, config->envp);
	}
	return (SUCCESS);
}

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	if (*status != CMD_SUCCEESS || node == NULL)
		return ;

	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("fork: %d\n", node->is_pipeline);
	printf("last_pipe_cmd : %d\n", node->is_last_pipe_cmd);

	/** (void)config; */
	/* -------------------------------------
	 * | NOTE : write execution code here  |
	 * ------------------------------------*/
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
	{
		// show error messege ...
		return ;
	}

	/** (3) make token to char **cmd_argv */
	char	**cmd_argv = get_cmd_argv(node->token);

	if (!(node->is_pipeline) && is_cd_or_exit_or_export(cmd_argv[0]))
		*status = exec_exceptions(node, cmd_argv, config); // fork없이 실행
	else
		*status = exec_general(node, cmd_argv, config); //  무조건 fork를 하는 애들.

	printf("EXECUTION finished... : %s\n", cmd_argv[0]);

	/** [> 마지막 커맨드의 실행 후 파일 디스크립터 복원 <] */
	/** if (node->is_last_pipe_cmd) */
	/** { */
		/** dup2(config->stdin_backup, STDIN_FILENO); */
		/** dup2(config->stdout_backup, STDOUT_FILENO); */
	/** } */

	printf("Delete cmd_argv... : %s\n", cmd_argv[0]);
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

	(void)config;

	status = CMD_SUCCEESS;

	printf("\n");
	printf("------------------------------------------\n");
	printf("|     Execution (inorder recursive)      |\n");
	printf("------------------------------------------\n");

	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node, config);

	// 여기서 마지막 pid_t를 기다린다.
	printf("execute() : waiting pid %d\n", config->last_cmd_pid);
	waitpid(config->last_cmd_pid, &wait_status, 0);
	printf("execute() : wstatus = %d\n", wait_status);
	printf("execute() : child's exit code = %d\n", (wait_status >> 8 & 0xff));

	/** 모든 노드 삭제 */
	inorder_recur(syntax_tree, &status, delete_tree_node, config);

	return (status);
}
