/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 21:52:20 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/30 18:48:41 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include <unistd.h>
# include <stdio.h>
# include "../../include/libft.h"

# define SUCCESS	(0)
# define ERROR		(1)

/**-
 * @ Deep copy system envp content.
 * - src/builtin/environ.c
 *   */
char	**new_environ(char **system_envp);

/**-
 * @ Find Key in envp list, then return pointer to Value. 
 * - NOTE: if no Key exists, then function returns NULL.
 * - src/builtin/environ.c
 **/
char	*get_environ_value(const char *env_key, char **envp);

/**- 
 * @ Free every environs.
 * - src/builtin/environ.c
 **/
void	delete_environ(char ***our_envp);

/**-
 * @ Export builtin function. 
 * - src/builtin/environ.c
 **/
int		exec_export(char **arglist, char ***our_envp_ptr);

/**- 
 * @ Env builtin function. 
 * - src/builtin/environ.c
 **/
int		exec_env(char **arglist, char **our_envp);

/**- 
 * @ Unset builtin function. 
 * - src/builtin/environ.c
 **/
int		exec_unset(char **arglist, char ***our_envp_ptr);

/* --------------------------------------------------- */

/**- 
 * @ Free char **strings.
 * - src/builtin/environ_utils.c 
 **/
void	delete_strs(char ***strs);

/* environ utils/helper functions */
size_t	get_strs_count(char **strs);
void	copy_strs(char **dst, char **src, size_t strs_count);
void	sort_ascii(int len, char **argv);
void	print_strs(char **strs);
void	print_in_order(char **our_envp);
void	add_to_envp(char *str, char ***our_envp_ptr);
void	copy_strs_for_export(char **dst, char **src, size_t strs_count);

#endif /* ENVIRON_H */
