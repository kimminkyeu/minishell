/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 21:59:30 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/21 21:03:49 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

# include "../src/builtin/environ.h"
# include "libft.h"

/* --------------------------------------------
 * |  NOTE : Environment variable functions   |
 * -------------------------------------------*/
/** deep copy system envp content */
char	**new_environ(char **system_envp);
/** free every environs */
void	delete_environ(char ***our_envp_ptr);

/* --------------------------------------------
 * |  NOTE : Builtin functions                |
 * -------------------------------------------*/

void	exec_export(char **arglist, char ***our_envp_ptr);

void	exec_env(char **arglist, char **our_envp);

void	exec_unset(char **arglist, char ***our_envp_ptr); 

void	exec_cd(char **arglist, char **our_envp);

void	exec_echo(char **arglist, char **our_envp);

void	exec_pwd(char **arglist, char **our_envp);

void	exec_exit(char **arglist, char **our_envp);

#endif /* BUILTIN_H */
