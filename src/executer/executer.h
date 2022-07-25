/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:19:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 19:30:35 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "../main/helper.h"
# include "../main/minishell.h"
# include "../parser/parse_tree.h"
# include "../../include/builtin.h"

/* only for cmd_execution */
# define CMD_SUCCEESS		(0)
# define CMD_FAILURE		(1)

/* only for stopping minishell loop */
# define CMD_STOP_SHELL		(-1)

/* Word Expand function */
int		expand_tokens(t_list *tokens, t_shell_config *config);

/* Token to arglist function */
char	*get_full_path(char *name, char **envp);
char	**get_cmd_argv(t_list *token);

int 	open_redirection(int *my_io, t_list *redir_list);
void 	set_redirection(int *my_io);
void	set_pipe(int *my_io, int *pipefd, t_shell_config *config, t_tree *node);

void	delete_tree_node(t_tree *node, int *status, t_shell_config *config);
void	execute_node(t_tree *node, int *status, t_shell_config *config);
void	inorder_recur(t_tree *node, int *status, void (*f)(t_tree *node, int *status, t_shell_config *config), t_shell_config *config);
int		execute(t_tree *syntax_tree, t_shell_config *config);


#endif /* EXECUTER_H */
