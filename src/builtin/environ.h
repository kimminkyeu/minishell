/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 21:52:20 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/20 21:58:40 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include <unistd.h>
# include <stdio.h>
# include "../../include/libft.h"

/** deep copy system envp content */
char	**new_environ(char **system_envp);

/** free every environs */
void	delete_environ(char ***our_envp);

/** export builtin function. */
void	exec_export(char **arglist, char **our_envp);

/** env builtin function. */
void	exec_env(char **arglist, char **our_envp);

/** unset builtin function. */
void	exec_unset(char **arglist, char **our_envp); 

/* --------------------------------------------------- */

/* helper functions */
size_t	get_strs_count(char **strs);
void	copy_strs(char **dst, char **src, size_t strs_count);
void	sort_ascii(int len, char **argv);
void	delete_strs(char ***strs);
void	print_strs(char **strs);
void	print_in_order(char **our_envp);
void	add_to_envp(char *str, char **our_envp);

#endif /* ENVIRON_H */
