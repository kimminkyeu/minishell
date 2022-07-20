/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 23:15:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/19 14:44:25 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/include/libft.h"
#include <unistd.h>//chdir()
#include <stdlib.h>//free()
#include <stdio.h>//perror()

/*
** cd with only a relative or absolute path.
** NOTE: chdir() work with both type.
*/

void	exec_cd(char **arglist)
{
	char	*message;

	if (chdir(arglist[1]) == -1)
	{
		message = ft_strjoin("lesh: cd: ", arglist[1]);
		perror(message);
		free(message);
	}
}

// main() for test
//gcc cd.c ../libft/src/ft_strjoin.c ../libft/src/ft_strlen.c

int	main()
{
	char **arglist = calloc(8, sizeof(char *));
	arglist[0] = "cd";
	arglist[1] = "/Users/yehan/minishell/src";
	// arglist[1] = "..";
	// arglist[1] = "/";
	// arglist[1] = "/Users/yehan/mini";

	exec_cd(arglist);

	char *buf = calloc(1, 100);
	getcwd(buf, 100);
	printf("%s\n", buf);
}