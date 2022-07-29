/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 15:22:12 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "executer.h"
#include "exec_command.h"
#include "../builtin/environ.h"
#include "../main/minishell.h"
#include "../lexer/token.h"
#include "../main/signal_handle.h"

extern int	g_is_sig_interupt;

void	delete_tree_node(t_tree *node, int *status, t_shell_config *config)
{
	(void)status;
	(void)config;

	if (node != NULL)
	{
		if ((node)->redirection != NULL)
			ft_lstclear(&node->redirection, delete_token);
		if (node->token != NULL)
			ft_lstclear(&node->token, delete_token);
		free(node);
		node = NULL;
	}
}

void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	t_token	*tok;
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
	if (cmd_argv != NULL && node->is_pipeline == false \
		&& is_builtin_func(cmd_argv[0]))
		*status = exec_exceptions(node, cmd_argv, config);
	else
		*status = exec_general(node, cmd_argv, config);//  무조건 fork를 하는 애들.
	if (cmd_argv != NULL)
		delete_strs(&cmd_argv);
}

/** 함수 포인터 글자수 줄이는 용도 */
typedef void(*t_callback_func)(t_tree *, int *, t_shell_config *);

void	inorder_recur(t_tree *node, int *status, t_callback_func callback, \
			t_shell_config *shell_config)
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

	//inorder_recur(syntax_tree, &status, count_node, config);
	/** 모든 노드 실행 */
	inorder_recur(syntax_tree, &status, execute_node, config);
	wait_every_pid(config);

	//heredoc fd 닫기

	/** FIXME : 모든 노드 삭제 --> 문제 발생.  */
	inorder_recur(syntax_tree, &status, delete_tree_node, config);

	return (WEXITSTATUS(config->last_cmd_wstatus));
}
