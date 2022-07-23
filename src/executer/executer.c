/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/23 18:26:55 by minkyeki         ###   ########.fr       */
/*   Updated: 2022/07/23 14:33:08 by minkyeki         ###   ########.fr       */
/*   Updated: 2022/07/22 13:20:52 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include "executer.h"
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
	if (ft_strncmp("cd", cmd, len + 1) \
			&& ft_strncmp("exit", cmd, len + 1) \
			&& ft_strncmp("export", cmd, len + 1))
		return (true);
	else
		return (false);
}

int	is_builtin_func()
{

}

int	exec_general(t_tree *node, char **cmd_argv, t_shell_config *config) // 무조건 fork를 하는 애들.
{
	pid_t	pid;
	t_pipe	pipe_fd2;

	/** if not last pipe cmd, run pipe() function. #PERROR is 1 */
	if (!node->is_last_pipe_cmd && pipe(pipe_fd2.data) == PIPE_ERROR)
		return (ERROR);// pipe function error...

	/** set_redirection은 어디로 가야 하나?  */
	if (!node->is_last_pipe_cmd) // 들어가기 전에 미리 파이프 설정.
		set_redirection(&pipe_fd2, node->redirection, config);

	pid = fork();
	if (pid == FORK_ERROR)
		return (ERROR);// fork() function error...



	if (pid != CHILD)
	{
		if (node->is_last_pipe_cmd)
			config->last_cmd_pid = pid; // save last_cmd's pid
		dup2(pipe_fd2.read, STDIN_FILENO);
		close(pipe_fd2.read);
		close(pipe_fd2.write);
	}
	else if (pid == CHILD) // if parent
	{
		
		if (is_builtin_func() == true)
			return (exec_builin()); //exit()
		if (ft_strchr(cmd_argv[0], '/') == NULL)
			cmd_argv[0] = get_full_path(cmd_argv[0]);
		if (cmd_argv[0] == NULL)
			//error
	 	execve(cmd_argv[0], cmd_argv, our_envp);
	}
}

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	if (*status != CMD_SUCCEESS)
		return ;

	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("fork: %d\n", node->is_pipeline);
	printf("last_pipe_cmd : %d\n", node->is_last_pipe_cmd);

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
		/** status = exec_exceptions(cmd_argv, node->redirection, config); // fork없이 실행 */
	else
		status = exec_general(node->redirection, cmd_argv, config); //  무조건 fork를 하는 애들.


	/** NOTE : if success, then set status to ... CMD_SUCCESS
	 *         else, set status to ...            CMD_FAILURE
	 *         if exit, then set status to ...    CMD_STOP_SHELL
	 * */

	/** *status = CMD_SUCCEESS; */
	/** *status = CMD_STOP_SHELL; */
	/** *status = CMD_FAILURE; */
	delete_strs(&cmd_argv);
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

	(void)config;

	status = CMD_SUCCEESS;

	printf("\n");
	printf("------------------------------------------\n");
	printf("|     Execution (inorder recursive)      |\n");
	printf("------------------------------------------\n");

	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node, config);

	/** 모든 노드 삭제 */
	inorder_recur(syntax_tree, &status, delete_tree_node, config);

	return (status);
}
