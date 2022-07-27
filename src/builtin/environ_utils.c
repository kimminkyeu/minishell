/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:23:45 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 22:05:56 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environ.h"
#include "../main/helper.h"

char	**new_environ(char **system_envp)
{
	size_t	env_count;
	char	**new_envp;

	env_count = get_strs_count(system_envp);
	new_envp = ft_calloc(env_count + 1, sizeof(*new_envp));
	copy_strs(new_envp, system_envp, env_count);
	return (new_envp);
}

void	delete_strs(char ***strs)
{
	size_t	i;

	if (strs == NULL || *strs == NULL || **strs == NULL)
		return ;
	i = 0;
	while ((*strs)[i] != NULL)
	{
		free((*strs)[i]);
		(*strs)[i] = NULL;
		i++;
	}
	free(*strs);
	*strs = NULL;
}

void	delete_environ(char ***our_envp)
{
	delete_strs(our_envp);
}

void	add_to_envp(char *str, char ***our_envp_ptr)
{
	size_t	strs_count;
	char	**new_envp;

	strs_count = get_strs_count(*our_envp_ptr);
	new_envp = ft_calloc(strs_count + 2, sizeof(*new_envp));
	copy_strs(new_envp, *our_envp_ptr, strs_count);
	delete_environ(our_envp_ptr);
	new_envp[strs_count] = ft_strdup(str);
	new_envp[strs_count + 1] = NULL;
	*our_envp_ptr = new_envp;
}

char	*get_environ_value(const char *env_key, char **envp)
{
	size_t	i;
	char	*target;

	i = 0;
	if (env_key == NULL || envp == NULL)
		return (NULL);
	target = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], env_key, ft_strlen(env_key)) == 0)
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return (target);
}
