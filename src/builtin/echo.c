/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:22:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 22:48:44 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "../libft/include/libft.h"

/*
** example: [echo] [-nnn] [a] [b] [-n] [c]
** 1) check 2th node
** 1-1) if NULL -> do nothing
** 1-2) if -n && only n detected -> -n option on
** 2) print from 3th to last
*/
#define SUCCESS (0)

static int	is_n_option(char *str)
{
	size_t	i;

	if (!ft_strncmp(str, "-n", 2))
	{
		i = 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (true);
	}
	return (false);
}

static void	write_arguments(char **arglist, size_t i)
{
	while (arglist[i])
	{
		ft_putstr_fd(arglist[i], STDOUT_FILENO);
		if (arglist[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	exec_echo(char **arglist, char **our_envp)
{
	int		n_option;

	(void)our_envp;
	n_option = false;
	if (arglist[1] != NULL)
	{
		if (is_n_option(arglist[1]) == true)
			n_option = true;
		write_arguments(arglist, 1 + n_option);
	}
	if (n_option == false)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
