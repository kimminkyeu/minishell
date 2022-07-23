/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/23 15:06:54 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "executer.h"
#include "minishell.h"
#include "token.h"

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

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	/** 아래 코드는 방문 순서를 확인하기 위한 코드일 뿐, 구현시엔 제거할 것. */

	if (*status != CMD_SUCCEESS)
		return ;

	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("fork: %d\n", node->is_pipeline);
	printf("last_pipe_cmd : %d\n", node->is_last_pipe_cmd);
	printf("\n\033[93mExecuting command...\033[0m\n\n");


	/* -------------------------------------
	 * | NOTE : write execution code here  |
	 * ------------------------------------*/

	/** (0) 명령어에 따라 분기하여 실행 */
	/* 전 과정: (pipe) - (fork) - redirection set - execute - (redirection restore)
	** 분기:
	** 1. 파이프라인이 아님 && cd나 exit: redir set - execute - redir restore
	** 2. 나머지 builtin: (pipe) - fork - redirection set - execute(status 수동)
	** 3. filesystem에서 실행하는 cmd: (pipe) - fork - redirection set - execute(status 자동)
	*/

	t_token *tok = node->token->content;

	/** if | or && or || or ( ), do not expand tokens. */
	if (tok->type != E_TYPE_SIMPLE_CMD)
	{
		/* handle_operation(t_shell_config *config)  */

		/** if [|] */

		/** if [&&] */

		/** if [||] */

		return ;
	}
	/** if E_TYPE_SIMPLE_CMD, then expand tokens */
	if (expand_tokens(node->token, config) == ERROR \
			|| expand_tokens(node->redirection, config) == ERROR)
	{
		// show error messege ...
		return ;
	}
	char	**cmd_argv = get_cmd_argv(node->token);
	printf("\n\033[93m#Making char **arglist...\033[0m\n");
	print_strs(cmd_argv);
	printf("\n");

	(void)cmd_argv;


/************************** set redir ******************************/

#include <fcntl.h>//open
#include "minishell.h"
#include "token.h"

# define IN 0
# define OUT 1

int	open_heredoc(const char *limiter)
{
	//여기는 pipex heredoc 코드에서 readline 부분만 바꾼 상태예요.
	//heredoc 관련 내용:
	// 노션 여기서 봐주세요. (https://www.notion.so/kyeu/Minishell-110e3b17e97e467eb44e1ef23b9e8882#df7f3307d1ce4456a6d3fbb7ca11e7b7)
	int			pipe_fd[2];
	pid_t		pid;
	const char	*line;

	pipe(pipe_fd);
	pid = fork();
	if (pid == CHILD)
	{
		close(pipe_fd[READ]);
		while (1)
		{
			write(2, "> ", 2);
			line = readline("\033[31mheredoc& \033[0m");
			if (is_limiter(line, limiter))
				exit(EXIT_SUCCESS);
			else
				write(pipe_fd[WRITE], line, ft_strlen(line));
		}
	}
	close(pipe_fd[WRITE]);
	wait(NULL);
	return (pipe_fd[READ]);
}

void	set_redirection(t_shell_config *shell, t_list *redir_list, char *new_io)// [>] [out] [<] [in]
{
	unsigned char	error_no;
	t_list			*cur;

	new_io[IN] = shell->stdin_backup;
	new_io[OUT] = shell->stdout_backup;
	error_no = 0;
	cur = redir_list;
	while (cur)
	{
		if (cur->content->type == E_TYPE_REDIR_LESS)
		{
			cur = cur->next;
			new_io[IN] = open(cur->content->str->text, O_RDONLY);
			if (new_io[IN] == -1)
				break ;
		}
		else if (cur->content->type == E_TYPE_REDIR_HEREDOC)
		{
			cur = cur->next;
			open_heredoc();
		}
		else if (cur->content->type == E_TYPE_REDIR_GREATER)
		{
			new_io[OUT] = open(cur->content->str->text, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (new_io[OUT] == -1)
				break ;
		}
		else if (cur->content->type == E_TYPE_REDIR_APPEND)
		{
			new_io[OUT] = open(cur->content->str->text, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (new_io[OUT] == -1)
				break ;
		}
		cur = cur->next;
	}
	if (cur)
	{
		//break로 나오면 open()이 실패해서 errno이 설정되었을 테고,
		//여기서 중단되고 에러메시지가 뜹니다.
		//다만, 뒤에 heredoc이 남아있을 경우에 모든 heredoc에 대한 입력을 받습니다.
		error_no = errno;
		while (cur)
		{
			if (ft_strncmp(redir_list->content->type, E_TYPE_REDIR_HEREDOC)
			{
				cur = cur->next;
				open_heredoc();
			}
		}
	}
	return (error_no);
	//exit() 해야 한다면 밖에서
}

/********************************************************/



	/** NOTE : if success, then set status to ... CMD_SUCCESS
	 *         else, set status to ...            CMD_FAILURE
	 *         if exit, then set status to ...    CMD_STOP_SHELL
	 * */

	/** *status = CMD_SUCCEESS; */
	/** *status = CMD_STOP_SHELL; */
	/** *status = CMD_FAILURE; */
	delete_strs(&cmd_argv);
	printf("-------------------\n");
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
