/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 21:52:20 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/26 16:05:55 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include <unistd.h>
# include <stdio.h>
# include "../../include/libft.h"

# define SUCCESS	(0)
# define ERROR		(1)

/** deep copy system envp content */
char	**new_environ(char **system_envp);

/** find Key in envp list, then return pointer to Value. 
 * NOTE : if no Key exists, then function returns NULL */
char	*get_environ_value(const char *env_key, char **envp);

/** free every environs */
void	delete_environ(char ***our_envp);

/** export builtin function. */
int		exec_export(char **arglist, char ***our_envp_ptr);

/** env builtin function. */
int		exec_env(char **arglist, char **our_envp);

/** unset builtin function. */
int		exec_unset(char **arglist, char ***our_envp_ptr); 

/* --------------------------------------------------- */

/* environ utils/helper functions */
size_t	get_strs_count(char **strs);
void	copy_strs(char **dst, char **src, size_t strs_count);
void	sort_ascii(int len, char **argv);

/* in builtin/environ_utils.c */
void	delete_strs(char ***strs);
void	print_strs(char **strs);
void	print_in_order(char **our_envp);
void	add_to_envp(char *str, char ***our_envp_ptr);

#endif /* ENVIRON_H */
