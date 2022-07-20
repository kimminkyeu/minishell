/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 12:46:16 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/20 13:15:22 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

/** deep copy envp content */
char	**new_environ(char **system_envp);
/** free environs */
void	delete_environ(char ***our_envp);

/** Builtin functions. */
void	exec_export(char **arglist, char **our_envp);

void	exec_env(char **arglist, char **our_envp);

void	exec_unset(char **arglist, char **our_envp); 


/**
 * (1) 모든 환경변수는 정렬하지 않고 그대로 저장한다. (뒤에)
 * (2) export 출력시 정렬 수행 후 출력한다. 
 *
 * */

/** -------------------------------------------------- */
/** 멤버 개수 확인 */
size_t	get_env_count(char **envp)
{
	size_t	cnt;
	size_t	i;

	cnt = 0;
	i = 0;
	while (envp[i] != NULL)
	{
		cnt++;
		i++;
	}
	return (cnt);
}

char	**new_environ(char **system_envp)
{
	size_t	i;
	// (1) 환경변수 불러와서 깊은 복사 실행
	size_t	env_count;

	env_count = get_env_count(system_envp);

	char	**new_envp = ft_calloc(env_count, sizeof(*new_environ));

	i = 0;
	while (i < env_count)
	{
		new_envp[i] = ft_strdup(system_envp[i]);
		i++;
	}
	new_envp[env_count] = NULL;
}

void	delete_environ(char ***our_envp)
{
	size_t	i;

	i = 0;
	while ((*our_envp)[i] != NULL)
	{
		free((*our_envp)[i]);
		(*our_envp)[i] = NULL;
		i++;
	}
	free(*our_envp);
	*our_envp = NULL;
}
