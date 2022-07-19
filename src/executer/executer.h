/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:19:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/19 23:39:35 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include "../main/helper.h"
# include "../parser/parse_tree.h"

# define CMD_FAILURE		(0)
# define CMD_SUCCEESS		(1)

void	delete_tree_node(t_tree *node);
int		execute_node(t_tree *node);
void	inorder_recur(t_tree *node, int *status);
int		execute(t_tree *syntax_tree);

#endif /* EXECUTER_H */
