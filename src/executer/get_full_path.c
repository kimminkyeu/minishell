/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_full_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 13:40:51 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/23 14:47:33 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** For opendir | readdir | closedir */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>

#include "../../include/libft.h"
#include "../main/minishell.h"
#include "../string/string.h"

# define COMMAND_NOT_FOUND (127)
/** # define ERROR		(-1) */
# define PERROR		(1)
# define PUTSTR		(0)

void	exit_error(char *messege, int is_perror, int exit_code)
{
	if (is_perror)
		perror(messege);
	else
		ft_putstr_fd(messege, STDERR_FILENO);
	exit(exit_code);
}

char	**get_environ_path(char	**envp)
{
	int		i;
	char	*p_path;
	char	**p_path_split;

	i = 0;
	p_path_split = NULL;
	while (envp[i] != NULL)
	{
		p_path = ft_strnstr(envp[i], "PATH", 4);
		if (p_path != NULL)
		{
			p_path_split = ft_split(p_path + 5, ':');
			if (p_path_split == NULL)
				exit_error("ft_split() :get_full_path.c", PUTSTR, EXIT_FAILURE);
			break ;
		}
		i++;
	}
	return (p_path_split);
}

/** 만약 그 경로에 프로그램이 있다면, true를 반환 */
int	is_valid_path(char *path, char *name)
{
	DIR				*dir_ptr;
	struct dirent	*file;
	int				is_valid_path;

	is_valid_path = false;
	if((dir_ptr = opendir(path)) == NULL)
		return (is_valid_path);
	while ((file = readdir(dir_ptr)) != NULL && is_valid_path == false)
	{
		/*  struct dirent *의 구조체에서 d_name 이외에는
		 *  시스템마다 항목이 없을 수 있으므로 무시하고 이름만 사용합니다. */
		if (ft_strncmp(file->d_name, name, ft_strlen(name) + 1) == 0)
			is_valid_path = true;
	}
	closedir(dir_ptr);
	return (is_valid_path);
}

/** 환경변수를 순회하면서, 프로그램 이름에 전부 붙여준다.
 * NOTE : 만약 못찾았다면, NULL을 반환한다. */
char	*get_full_path(char *name, char **envp)
{
	t_string	*buffer;
	char		**env_path;
	char		*cmd_full_path;
	size_t		i;

	cmd_full_path = NULL;
	env_path = get_environ_path(envp);
	buffer = new_string(20);
	i = -1;
	while (env_path[++i])
	{
		buffer->f_append(buffer, env_path[i]);
		if (is_valid_path(buffer->text, name))
		{
			buffer->f_push_back(buffer, '/');
			buffer->f_append(buffer, name);
			cmd_full_path = ft_strdup(buffer->text);
			break ;
		}
		buffer->f_clear(buffer);
	}
	free(env_path);
	delete_string(&buffer);
	return (cmd_full_path);
}
