/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 01:57:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 15:13:05 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

/** Standard library */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

#include "minishell.h"
#include "helper.h"
#include "../../include/builtin.h"
#include "../../include/libft.h"

extern int	g_is_sig_interupt;

/** NOTE : Use of waitpid() is to make sure
 *         sig_handler run only at main process. 
 **/
void	sig_ctrl_c(int signal)
{
	int	pid;

	pid = waitpid(-1, NULL, WNOHANG);
	if (signal == SIGINT)
	{
		g_is_sig_interupt = true;
		if (pid == -1)
		{
			rl_replace_line("", 1);
			ft_putstr_fd("\n", STDOUT_FILENO);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

void	set_signal(void)
{
	signal(SIGINT, sig_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void	*new_pid(pid_t pid)
{
	pid_t	*tmp;

	tmp = ft_calloc(1, sizeof(*tmp));
	*tmp = pid;
	return (tmp);
}

static void	kill_pid_list(t_list *cur)
{
	pid_t	cur_pid;

	while (cur != NULL)
	{
		cur_pid = *((pid_t *)cur->content);
		kill(cur_pid, SIGTERM);
		cur = cur->next;
	}
	g_is_sig_interupt = false;
}

void	wait_every_pid(t_shell_config *config)
{
	t_list	*cur;
	pid_t	cur_pid;

	cur = config->pid_list;
	while (cur != NULL)
	{
		cur_pid = *((pid_t *)cur->content);
		if (cur_pid == config->last_cmd_pid)
			waitpid(cur_pid, &config->last_cmd_wstatus, 0);
		else
			waitpid(cur_pid, NULL, 0);
		if (g_is_sig_interupt == true)
		{
			kill_pid_list(config->pid_list);
			break ;
		}
		cur = cur->next;
	}
	ft_lstclear(&config->pid_list, free);
}
