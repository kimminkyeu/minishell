/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 23:15:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 22:16:59 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** getcwd(), STDOUT_FILENO */
#include <unistd.h>
/** define MAXPATHLEN 1024 (man getwd) */
#include <sys/param.h>
/** chdir() */
#include <unistd.h>
/** free() */
#include <stdlib.h>
/** perror() */
#include <stdio.h>

#include "../libft/include/libft.h"
#include "../../include/builtin.h"

/*
** cd with only a relative or absolute path.
** NOTE: chdir() work with both type.
*/

#define NO_SUCH_DIRECTORY (2)

/**
 * NOTE : if cmd is [cd -] or [cd ~], then substitute arglist[1]. 
 * [ cd ~/minishell/include ] should work to. */
void	set_environ_directory_to_arglist(char **arglist, char ***envp_ptr)
{
	char	*path;

	path = NULL;
	if (arglist[1][0] == '-' && arglist[1][1] == '\0')
	{
		free(arglist[1]);
		path = get_environ_value("OLDPWD", *envp_ptr);
		arglist[1] = ft_strdup(path);
	}
	else if (arglist[1][0] == '~' 
			&& (arglist[1][1] == '\0' || arglist[1][1] == '/'))
	{
		path = get_environ_value("HOME", *envp_ptr);
		path = ft_strjoin(path, arglist[1] + 1);
		free(arglist[1]);
		arglist[1] = path;
	}
}

/** before cd, set OLDPWD to current directory */
void	update_old_pwd(char **arglist, char ***envp_ptr)
{
	char	*message;
	char	*path;
	char	*tmp;

	message = ft_calloc(1, MAXPATHLEN);
	getcwd(message, MAXPATHLEN);
	path = ft_strjoin("OLDPWD=", message);
	free(message);
	tmp = arglist[1];
	arglist[1] = path;
	exec_export(arglist, envp_ptr);
	free(path);
	arglist[1] = tmp;
}

void	reset_pwd(char ***envp_ptr)
{
	char	*path_old;
	char	*path_new;
	char	**arglist_new;

	arglist_new = ft_calloc(3, sizeof(*arglist_new));
	arglist_new[0] = ft_strdup("export");
	path_old = ft_strjoin("PWD=", get_environ_value("HOME", *envp_ptr));
	path_new = ft_strjoin("OLDPWD=", get_environ_value("HOME", *envp_ptr));
	arglist_new[1] = path_old;
	exec_export(arglist_new, envp_ptr);
	free(arglist_new[1]);
	arglist_new[1] = path_new;
	exec_export(arglist_new, envp_ptr);

	/** FIXME : Possible Memory Leak. */
	free(arglist_new[0]);
	free(arglist_new[1]);
	free(arglist_new);
}

/** change current directory path. */
int	exec_cd(char **arglist, char ***envp_ptr)
{
	char	*message;
	char	*path;

	if (arglist[1] == NULL)
	{
		reset_pwd(envp_ptr);
		return (SUCCESS);
	}
	if (arglist[1][0] == '-' || arglist[1][0] == '~')
		set_environ_directory_to_arglist(arglist, envp_ptr);
	update_old_pwd(arglist, envp_ptr);
	if (chdir(arglist[1]) == -1)
	{
		message = ft_strjoin("lesh: cd: ", arglist[1]);
		perror(message);
		free(message);
		return (NO_SUCH_DIRECTORY);
	}
	message = ft_calloc(1, MAXPATHLEN);
	getcwd(message, MAXPATHLEN);
	path = ft_strjoin("PWD=", message);
	free(message);
	free(arglist[1]);
	arglist[1] = path;
	exec_export(arglist, envp_ptr);
	return (SUCCESS);
}
