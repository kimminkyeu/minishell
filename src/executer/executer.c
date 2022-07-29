/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:15:09 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 17:09:22 by yehan            ###   ########seoul.kr  */
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

/** execute_node():
 ** STEPS:
 ** 1) if | or && or || or ( ), run exec_priority_operator()
 **/
void	execute_node(t_tree *node, int *status, t_shell_config *config)
{
	t_token	*tok;
	char	**cmd_argv;

	if (*status != CMD_KEEP_RUNNING || node == NULL)
		return ;
	tok = NULL;
	if (node->token != NULL)
		tok = node->token->content;
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
		*status = exec_general(node, cmd_argv, config);
	if (cmd_argv != NULL)
		delete_strs(&cmd_argv);
}

/** inorder_recur():
 ** NOTES:
 ** 1) this function can do both execute and delete node.
 ** 2) if status == CMD_STOP_SHELL (ex. calling exit) then stop all
 ** STEPS:
 ** 1) if no callback argument, execute every node
 ** 2) if delete node function comes in, delete every node
 **/

typedef void(*t_callback_func)(t_tree *, int *, t_shell_config *);

void	inorder_recur(t_tree *node, int *status, t_callback_func callback, \
			t_shell_config *shell_config)
{
	if (node == NULL)
		return ;
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

/** execute():
 ** NOTES:
 ** 1) Executer function. make traversing.
 ** STEPS:
 ** 1) execute every node
 ** 2) delete every node
 **/
int	execute(t_tree *syntax_tree, t_shell_config *config)
{
	int	status;

	status = CMD_KEEP_RUNNING;
	inorder_recur(syntax_tree, &status, execute_node, config);
	wait_every_pid(config);
	inorder_recur(syntax_tree, &status, delete_tree_node, config);
	return (WEXITSTATUS(config->last_cmd_wstatus));
}
