/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/17 22:24:38 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/21 14:36:46 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../src/executer/executer.h"

/* @ Traverse tokens lists, then execute each node's simple command.
 * returns (0) on error. 
 * else, function returns (1):success
 * */

extern int	execute(t_tree *syntax_tree, t_shell_config *config);

#endif /* EXECUTE_H */
