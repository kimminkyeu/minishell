/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 21:22:09 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "executer.h"
#include "../builtin/environ.h"
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
		node = NULL;
	}
}

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

	pipe_fd[READ] = config->stdin_backup;
	pipe_fd[WRITE] = config->stdout_backup;
	status = open_redirection(pipe_fd, node->redirection);
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
	status = exec_builtin(cmd_argv, config->envp);
	if (pipe_fd[READ] != config->stdin_backup)
		dup2(config->stdin_backup, STDIN_FILENO);
	if (pipe_fd[WRITE] != config->stdout_backup)
		dup2(config->stdout_backup, STDOUT_FILENO);
	return (status);
}

/** Function for exec_general */
void	run_child_process(char **cmd_argv, t_shell_config *config)
{
	char	*full_path;

	full_path = NULL;
	if (cmd_argv == NULL)
		exit(SUCCESS);
	if (is_builtin_func(cmd_argv[0]) == true)
	{
		exit(exec_builtin(cmd_argv, config->envp));
	}
	/** ./minishell */
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

int	exec_general(t_tree *node, char **cmd_argv, t_shell_config *config)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		tmp_fd;

	if (pipe(pipe_fd) == PIPE_ERROR)
	{
		perror("pipe()");
		return (ERROR);
	}
	config->num_of_child_process += 1;
	pid = fork();
	if (pid == FORK_ERROR)
	{
		perror("fork()");
		return (ERROR);
	}	
	if (pid == CHILD) /* child */
	{
		/** 마지막 커맨드라면, 파이프의 쓰기 기본값은 표준출력이여야 한다. */
		if (node->is_last_pipe_cmd)
			dup2(config->stdout_backup, pipe_fd[WRITE]);

		/** NOTE : 애먹은 부분이다.  
		 * 만약 open_redirection으로 pipe_fd[READ]가 바뀌었다면,
		 * redirection_in이 있다는 이야기이므로 이때만 dup2로 stdin을 변조시킨다.
		 * 만약 바뀌지 않았다면 dup2를 실행하면 안된다. */
		tmp_fd = pipe_fd[READ];
		open_redirection(pipe_fd, node->redirection);
		if (tmp_fd != pipe_fd[READ])
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
		}
		if (!node->is_last_pipe_cmd)
		{
			/** 마지막 커맨드가 아니라면, 부모는 항상 다음 커맨드의 STDIN을 연결해준다.  */
			dup2(pipe_fd[READ], STDIN_FILENO);
		}
		close(pipe_fd[WRITE]);
		close(pipe_fd[READ]);
	}
	return (SUCCESS);
}


/** exec_subshell은 무조건 fork로 실행. --> 기본 구조는 exec_general 과 코드가 동일.
 * */
void	exec_priority_operator(t_tree *node, t_token *tok, int *status, t_shell_config *config)
{
	int	i;

	/** (1-1) if [() : subshell] */
	if (tok->type == E_TYPE_BRACKET)
		*status = exec_subshell(node, tok->str, config);
	/** (1-3) if [&& : waitpid] */
	else if (tok->type == E_TYPE_DOUBLE_AMPERSAND)
	{
		waitpid(config->last_cmd_pid, &config->last_cmd_wstatus, 0);
		i = -1;
		while (++i < config->num_of_child_process - 1)
			wait(NULL);
		if (WEXITSTATUS(config->last_cmd_wstatus) != SUCCESS)
			*status = CMD_STOP_RUNNING; // NOTE : stop running other process.
	}
	/** (1-4) if [|| : waitpid] */
	else if (tok->type == E_TYPE_DOUBLE_PIPE)
	{
		waitpid(config->last_cmd_pid, &config->last_cmd_wstatus, 0);
		i = -1;
		while (++i < config->num_of_child_process - 1)
			wait(NULL);
		if (WEXITSTATUS(config->last_cmd_wstatus) == SUCCESS)
			*status = CMD_STOP_RUNNING; // NOTE : stop running other process.
	}
	return ;
}

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	t_token *tok;
	char	**cmd_argv;

	if (*status != CMD_KEEP_RUNNING || node == NULL)
		return ;
	tok = NULL;
	if (node->token != NULL)
		tok = node->token->content;

	/** (1) if | or && or || or ( ) parenthethis for priority */
	if (tok != NULL && tok->type != E_TYPE_SIMPLE_CMD)
		return (exec_priority_operator(node, tok, status, config));
	else if (expand_tokens(node->token, config) == ERROR \
			|| expand_tokens(node->redirection, config) == ERROR)
		return ;

	cmd_argv = get_cmd_argv(node->token);
	if (cmd_argv != NULL && node->is_pipeline == false && is_builtin_func(cmd_argv[0]))
		*status = exec_exceptions(node, cmd_argv, config);
	else
		*status = exec_general(node, cmd_argv, config); //  무조건 fork를 하는 애들.
	if (cmd_argv != NULL)
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
	int	i;

	status = CMD_KEEP_RUNNING;

	//inorder_recur(syntax_tree, &status, count_node, config);
	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node, config);
	waitpid(config->last_cmd_pid, &config->last_cmd_wstatus, 0);
	i = -1;
	while (++i < config->num_of_child_process - 1)
		wait(NULL);

	/** FIXME : 모든 노드 삭제 --> 문제 발생.  */
	inorder_recur(syntax_tree, &status, delete_tree_node, config);

	return (WEXITSTATUS(config->last_cmd_wstatus));
}
