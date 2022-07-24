/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 23:15:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 00:27:43 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>//getcwd(), STDOUT
#include <sys/param.h>//define MAXPATHLEN 1024 (man getwd)
#include <unistd.h>//chdir()
#include <stdlib.h>//free()
#include <stdio.h>//perror()
#include "../libft/include/libft.h"
#include "../../include/builtin.h"

/*
** cd with only a relative or absolute path.
** NOTE: chdir() work with both type.
*/

#define NO_SUCH_DIRECTORY (2)

int	exec_cd(char **arglist, char ***envp_ptr)
{
	char	*message;
	char	*path;

	if (arglist[1] == NULL)
		return (SUCCESS);

	if (arglist[1][0] == '-')
	{
		/** old_pwd를 불러와서 arglist[1]로 치환.*/
		free(arglist[1]);
		path = get_environ_value("OLDPWD", *envp_ptr);
		arglist[1] = ft_strdup(path);
	}
	/* (1) 현재 경로를 old_pwd에 저장. */
	message = ft_calloc(1, MAXPATHLEN);
	getcwd(message, MAXPATHLEN);
	path = ft_strjoin("OLDPWD=", message);
	free(message);
	char *tmp = arglist[1];
	/** free(arglist[1]); */
	arglist[1] = path;
	exec_export(arglist, envp_ptr);
	free(path);
	arglist[1] = tmp;

/** (2) 이동 실행.  */
	if (chdir(arglist[1]) == -1)
	{
		message = ft_strjoin("lesh: cd: ", arglist[1]);
		perror(message);
		free(message);
		return (NO_SUCH_DIRECTORY);
	}
	else
	{
		/** 이동 성공시, pwd에 현재 경로 저장.  */
		message = ft_calloc(1, MAXPATHLEN);
		getcwd(message, MAXPATHLEN);
		path = ft_strjoin("PWD=", message);
		free(message);
		free(arglist[1]);
		arglist[1] = path;
		exec_export(arglist, envp_ptr);
	}
	return (SUCCESS);
}


// main() for test
//gcc cd.c ../libft/src/ft_strjoin.c ../libft/src/ft_strlen.c

/** int	main()
  * {
  *     char **arglist = calloc(8, sizeof(char *));
  *     arglist[0] = "cd";
  *     arglist[1] = "/Users/yehan/minishell/src";
  *     // arglist[1] = "..";
  *     // arglist[1] = "/";
  *     // arglist[1] = "/Users/yehan/mini";
  *
  *     exec_cd(arglist);
  *
  *     char *buf = calloc(1, 100);
  *     getcwd(buf, 100);
  *     printf("%s\n", buf);
  * } */
