/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:19:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/26 23:13:35 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "../main/helper.h"
# include "../main/minishell.h"
# include "../parser/parse_tree.h"
# include "../../include/builtin.h"

/* only for cmd_execution */
# define CMD_KEEP_RUNNING	(0)
# define CMD_STOP_RUNNING	(1)

/* only for stopping minishell loop */
# define CMD_STOP_SHELL		(-1)

/**-
 * @ Word Expansion function 
 * - src/executer/token_expand.c
 **/
int		expand_tokens(t_list *tokens, t_shell_config *config);

/**- 
 * @ Get program's full path.
 * - Returns NULL if program doesn't exist.
 * - src/executer/get_full_path.c
 **/
char	*get_full_path(char *name, char **envp);

/**- 
 * @ Convert token to argv (char **argv)
 * - src/executer/token_expand.c 
 **/
char	**get_cmd_argv(t_list *token);

/**-
 * @ Set file descripters to pipe_fd[2] from redirection_list.
 * - src/executer/redirection.c
 **/
int 	open_redirection(int *pipe_fd, t_list *redir_list);

void	delete_tree_node(t_tree *node, int *status, t_shell_config *config);
void	execute_node(t_tree *node, int *status, t_shell_config *config);
void	inorder_recur(t_tree *node, int *status, void (*f)(t_tree *node, int *status, t_shell_config *config), t_shell_config *config);
int		execute(t_tree *syntax_tree, t_shell_config *config);

#endif /* EXECUTER_H */
