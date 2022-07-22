/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:19:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 15:50:27 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "../main/helper.h"
# include "../parser/parse_tree.h"
# include "../../include/builtin.h"

/* only for cmd_execution */
# define CMD_FAILURE		(0)
# define CMD_SUCCEESS		(1)

/* only for stopping minishell loop */
# define CMD_STOP_SHELL		(-1)

/* typedef union u_pipe {
 *     struct {
 *         int	read;
 *         int	write;
 *     };
 *     int	fd[2];
 * }	t_pipe; */

typedef struct s_shell_config {
	char	**envp;
	int		stdin_backup;
	int		stdout_backup;
	

} t_shell_config;

/* Word Expand function */
int		expand_token(t_list *tokens, t_shell_config *config);


void	delete_tree_node(t_tree *node, int *status, t_shell_config *config);
void	execute_node(t_tree *node, int *status, t_shell_config *config);
void	inorder_recur(t_tree *node, int *status, void (*f)(t_tree *node, int *status, t_shell_config *config), t_shell_config *config);
int		execute(t_tree *syntax_tree, t_shell_config *config);


#endif /* EXECUTER_H */
