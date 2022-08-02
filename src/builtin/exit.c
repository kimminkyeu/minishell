/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:30:26 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 15:15:09 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../libft/include/libft.h"
#include "../main/minishell.h"

/* exit [n]: exit with status of n.
** 1. no [n]: exit with exit status of last command.
** 2-1. [n] more than 1: command exit status 1. !!exit fail!!
** 2-2. [n] is not number: exit(255).
*/

/* how exit() work with argument:
** 1) atoll(alpha to longlong)
** 1-1) if over/underflow occurs, it's not a number.
** 2) cast into unsigned char
*/

static bool	get_status(char *str, unsigned char *status)
{
	int					neg;
	unsigned long long	num;
	size_t				i;

	i = 0;
	neg = 1;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	num = 0;
	while ('0' <= str[i] && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		if ((neg == -1 && num > (unsigned long long)LLONG_MIN)
			|| (neg == 1 && num > (unsigned long long)LLONG_MAX))
			return (false);
		i++;
	}
	if (str[i] == '\0')
	{
		*status = neg * num;
		return (true);
	}
	return (false);
}

int	exec_exit(char **arglist, char **our_envp, t_shell_config *config)
{
	unsigned char	status;

	(void)our_envp;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (arglist[1] == NULL)
		status = config->last_cmd_wstatus;
	else if (get_status(arglist[1], &status) == false)
	{
		status = 255;
		ft_putstr_fd("lesh: exit: ", STDERR_FILENO);
		ft_putstr_fd(arglist[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	}
	else if (arglist[2] != NULL)
	{
		ft_putstr_fd("lesh: exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	exit(status);
}
