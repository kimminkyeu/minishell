#include "../main/minishell.h"
#include "../parser/parse_tree.h"
#include "../main/signal_handle.h"
#include "exec_command.h"
#include "executer.h"

# define EXIT_COMMAND_NOT_FOUND (127)

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


/** Function for exec_general */
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

void	child_process(int *pipe_fd, t_tree *node, char **cmd_argv, t_shell_config *config)
{
	int		tmp_fd;

/** 마지막 커맨드라면, 파이프의 쓰기 기본값은 표준출력이여야 한다. */
	if (node->is_last_pipe_cmd)
		dup2(config->stdout_backup, pipe_fd[WRITE]);

	/** NOTE : 애먹은 부분이다.
	 * 만약 open_redirection으로 pipe_fd[READ]가 바뀌었다면,
	 * redirection_in이 있다는 이야기이므로 이때만 dup2로 stdin을 변조시킨다.
	 * 만약 바뀌지 않았다면 dup2를 실행하면 안된다. */
	tmp_fd = pipe_fd[READ];
	open_redirection(pipe_fd, node->redirection, config);
	if (tmp_fd != pipe_fd[READ])
		dup2(pipe_fd[READ], STDIN_FILENO);
	/** 여긴 확실. 세팅된 파이프를 표준 출력에 덮어쓴다. */
	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[WRITE]);
	close(pipe_fd[READ]);
	/** 최종 설정된 stdin stdout을 그대로 실행한다. */
	exec_command(cmd_argv, config);
}

void	parent_process(int *pipe_fd, pid_t pid, t_tree *node, t_shell_config *config)
{
	/** ctrl+c 시그널 발생시 모든 자식 프로세스 종료를 위해 리스트로 변경함. */
	ft_lstadd_back(&config->pid_list, ft_lstnew(new_pid(pid)));
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
