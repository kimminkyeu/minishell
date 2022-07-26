/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/26 17:47:04 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define READ			(0)
# define WRITE			(1)

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
	int		pipe_fd[2];
	int		status;

	/** printf("\033[90mcalling exec_exceptions() : %s\033[0m\n\n", cmd_argv[0]); */

	status = open_redirection(pipe_fd, node->redirection);

	dup2(pipe_fd[READ], STDIN_FILENO);
	close(pipe_fd[READ]);

	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[WRITE]);

	if (status != 0)
		return (status);
	status = exec_builtin(cmd_argv, config->envp);

	dup2(config->stdin_backup, STDIN_FILENO);
	dup2(config->stdout_backup, STDOUT_FILENO);

	return (status);
}

void	run_child_process(char **cmd_argv, t_shell_config *config)
{
	char	*full_path;

	if (cmd_argv == NULL)
		exit(SUCCESS);
	if (is_builtin_func(cmd_argv[0]) == true)
	{
		int status = exec_builtin(cmd_argv, config->envp);
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



int	exec_general(t_tree *node, char **cmd_argv, t_shell_config *config)
{
	pid_t	pid;
	int		pipe_fd[2];


	/** 파이프 열기. */
	if (pipe(pipe_fd) == PIPE_ERROR)
	{
		perror("pipe()");
		return (ERROR);
	}


	/** 파이프 열고 포크를 뜨면, 두 프로세스가 같은 파이프를 같게 된다.  */
	pid = fork();
	if (pid == FORK_ERROR)
	{
		perror("fork()");
		return (ERROR);
	}


	/** 
	 * -------------------------------------------------------------------------------
	 * | FIXME BOARD                                                                 |
	 * -------------------------------------------------------------------------------
	 *
	 * @ FIXME [ <in cat ] cat으로 <in이 전달이 안됨.
	 * 수정완료 --> | 자식프로세스에서 dup2()로 덮어쓰니 된다. 
	 *
	 * @ FIXME [ <in ls >out | <in grep minishell ] : core dumpe 에러.  
	 * 수정완료 --> | 다음에 <가 등장해도 되므로 토큰상 에러처리 하면 안됨. (원래 했었음)
	 *
	 * @ FIXME [ ls -al | grep minishell ] 을 여러번 치면 readline에 그 내용이 들어갈까?
	 * 수정완료 --> wait(NULL) 로 모든 자식프로세스를 기다리게 하니 해결됬다. 
	 *
	 * @ FIXME [ <in ] 만 들어왔을 때 redirection이 실행 되어야 한다.
	 * 수정완료 --> command가 없어도, set_redirection 함수를 호출하도록 로직 변경.
	 *
	 * @ FIXME [ ls -al | <out2 grep "my name" >out4 ] 하면 grep에 my만 들어옴. 
	 *         name이 안들어오고 터짐. 고칠것. 
	 *         정상 bash 결과 : grep 이 "my name"을 찾아야 함.
	 * 수정완료 --> expand_token.c 의 get_arglist()함수 로직이 문제였음. ft_split 사용안하니 해결.
	 *
	 *
	 * ------------------------------------------------------------------------------*/

	if (pid == CHILD) /* child */
	{
	
		/** 마지막 커맨드라면, 파이프의 쓰기 기본값은 표준출력이여야 한다. */
		if (node->is_last_pipe_cmd)
			dup2(config->stdout_backup, pipe_fd[WRITE]);

		int	tmp1 = pipe_fd[READ];


		/** NOTE : 애먹은 부분이다.  
		 * 만약 open_redirection으로 pipe_fd[READ]가 바뀌었다면,
		 * redirection_in이 있다는 이야기이므로 이때만 dup2로 stdin을 변조시킨다.
		 * 만약 바뀌지 않았다면 dup2를 실행하면 안된다. */
		open_redirection(pipe_fd, node->redirection);
		if (tmp1 != pipe_fd[READ])
			dup2(pipe_fd[READ], STDIN_FILENO);



		/** 여긴 확실. 세팅된 파이프를 표준 출력에 덮어쓴다. */
		dup2(pipe_fd[WRITE], STDOUT_FILENO);



		close(pipe_fd[WRITE]);
		close(pipe_fd[READ]);

		/** 최종 설정된 stdin stdout을 그대로 실행한다. */
		run_child_process(cmd_argv, config);
	}
	else /* parent */
	{
		if (node->is_last_pipe_cmd)
		{
			/* 마지막 커맨드의 부모에서 변조된 표준입출력을 복구한다. */
			config->last_cmd_pid = pid;
			dup2(config->stdin_backup, STDIN_FILENO);
			dup2(config->stdout_backup, STDOUT_FILENO);
			close(pipe_fd[WRITE]);
			close(pipe_fd[READ]);
		}
		if (!node->is_last_pipe_cmd)
		{
			/** 마지막 커맨드가 아니라면, 부모는 항상 다음 커맨드의 STDIN을 연결해준다.  */
			dup2(pipe_fd[READ], STDIN_FILENO);
			close(pipe_fd[WRITE]);
			close(pipe_fd[READ]);
		}
	}
	return (SUCCESS);
}

/** NOTE: subshell 코드는 exec_general의 코드와 동일함.
 * 함수 포인터를 마지막 인자로 받게 해서 두 함수를 하나로 줄일 수 있을 듯. 
 * */
int	exec_subshell(t_tree *node, t_string *str, t_shell_config *config)
{
	printf("Running Subshell : [%s]\n", str->text);
	(void)node;
	(void)config;
	// ...
	return (SUCCESS);
}

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	if (*status != CMD_KEEP_RUNNING || node == NULL)
		return ;

	t_token *tok;

	tok = NULL;
	if (node->token != NULL)
		tok = node->token->content;


	/** (1) if | or && or || or ( ), do not expand tokens. */
	if (tok != NULL && tok->type != E_TYPE_SIMPLE_CMD)
	{
		/** (1-1) if [() : subshell] */
		if (tok->type == E_TYPE_BRACKET)
		{
			*status = exec_subshell(node, tok->str, config);
		}
		/** (1-3) if [&& : waitpid] */
		else if (tok->type == E_TYPE_DOUBLE_AMPERSAND)
		{
			waitpid(config->last_cmd_pid, &config->last_cmd_wstatus, 0);
			wait(NULL);
			if (WEXITSTATUS(config->last_cmd_wstatus) != SUCCESS)
				*status = CMD_STOP_RUNNING; // NOTE : stop running other process.
		}
		/** (1-4) if [|| : waitpid] */
		else if (tok->type == E_TYPE_DOUBLE_PIPE)
		{
			waitpid(config->last_cmd_pid, &config->last_cmd_wstatus, 0);
			wait(NULL);
			if (WEXITSTATUS(config->last_cmd_wstatus) == SUCCESS)
				*status = CMD_STOP_RUNNING; // NOTE : stop running other process.
		}
		return ;
	}
	else if (expand_tokens(node->token, config) == ERROR \
			|| expand_tokens(node->redirection, config) == ERROR)
		return ;


	char	**cmd_argv = get_cmd_argv(node->token);



	if (cmd_argv != NULL && node->is_pipeline == false && is_builtin_func(cmd_argv[0]))
		*status = exec_exceptions(node, cmd_argv, config);
	else
		*status = exec_general(node, cmd_argv, config); //  무조건 fork를 하는 애들.

	
	if (cmd_argv != NULL)
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
	if (callback != delete_tree_node && *status == CMD_KEEP_RUNNING)
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

	status = CMD_KEEP_RUNNING;

	/** printf("\n"); */
	/** printf("------------------------------------------\n"); */
	/** printf("|     Execution (inorder recursive)      |\n"); */
	/** printf("------------------------------------------\n\n"); */
    /**  */
	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node, config);
	

	/** 0726 NOTE : 
	 * 가장 중요한 문제. 
	 * wait을 모두 해주지 않아 자식 프로세스의 응답이 늦게 stdin으로 출력되고
	 * 이게 꼬여서 결국 실행되지 않는 거였어...*/
	waitpid(config->last_cmd_pid, &config->last_cmd_wstatus, 0);
	wait(NULL);



	/** printf("\n"); */
	/** printf("\033[90mexecute() : waiting pid %d\033[0m\n", config->last_cmd_pid); */
	/** printf("\033[90mexecute() : child's exit code = %d\033[0m\n", WEXITSTATUS(config->last_cmd_wstatus)); */
	/** printf("\n\n"); */

	/** 모든 노드 삭제 */
	inorder_recur(syntax_tree, &status, delete_tree_node, config);

	return (status);
}
