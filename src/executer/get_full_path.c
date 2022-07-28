/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_full_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 13:40:51 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 17:29:07 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** For opendir | readdir | closedir */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>

#include "../builtin/environ.h"
#include "../../include/libft.h"
#include "../main/minishell.h"
#include "../string/string.h"

#define COMMAND_NOT_FOUND (127)
/** # define ERROR		(-1) */
#define PERROR		(1)
#define PUTSTR		(0)

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

	/** if (name == NULL) */
		/** return (false); */
	is_valid_path = false;
	dir_ptr = opendir(path);
	if (dir_ptr == NULL)
		return (is_valid_path);
	while (is_valid_path == false)
	{
		file = readdir(dir_ptr);
		if (file != NULL)
			break ;
		/*  struct dirent *의 구조체에서 d_name 이외에는
		 *  시스템마다 항목이 없을 수 있으므로 무시하고 이름만 사용합니다. */
		if (ft_strncmp(file->d_name, name, ft_strlen(name) + 1) == 0)
			is_valid_path = true;
	}
	closedir(dir_ptr);
	return (is_valid_path);
}

char	*get_full_path(char *name, char **envp)
{
	t_string	*buffer;
	char		**env_path;
	char		*cmd_full_path;
	size_t		i;

	i = -1;
	buffer = new_string(20);
	cmd_full_path = get_environ_value("PWD", envp);

	/** ./minishell 도 처리하기 위함. */
	if (ft_strchr(name, '/') != NULL && is_valid_path(cmd_full_path, ft_strchr(name, '/') + 1))
	{
		buffer->f_append(buffer, name);
		buffer->f_replace_all(buffer, ".", cmd_full_path);
		cmd_full_path = ft_strdup(buffer->text);
		delete_string(&buffer);
		return (cmd_full_path);
	}
	cmd_full_path = NULL;
	env_path = get_environ_path(envp);
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
