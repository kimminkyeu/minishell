/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:33:02 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 10:59:42 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/param.h>
#include <stdio.h>
#include "../libft/include/libft.h"

/*
** NOTE1: It is independent of environment variable PWD.
** NOTE2: All arguments are ignored.
*/

#define SUCCESS (0)
#define ERROR	(1)

int	exec_pwd(char **arglist, char **our_envp)
{
	char	*buf;

	(void)arglist;
	(void)our_envp;
	buf = ft_calloc(1, MAXPATHLEN);
	if (getcwd(buf, MAXPATHLEN) == NULL)
	{
		perror("lesh: pwd");
		return (ERROR);
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(buf);
	return (SUCCESS);
}
