/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 01:57:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 15:43:46 by yehan            ###   ########seoul.kr  */
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

extern int g_is_sig_interupt;

void	sig_ctrl_c(int signal)
{
	int pid;

	pid = waitpid(-1, NULL, WNOHANG);
	if (signal == SIGINT)
	{
		if (pid == -1)
		{
			g_is_sig_interupt = true;
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
	signal(SIGINT, sig_ctrl_c);		// FIXME : CTRL + C --> $?가 1로 바뀌어야 한다.
	signal(SIGQUIT, SIG_IGN);		// CTRL + / -> SIG_IGN = signal 무시.
}


void	*new_pid(pid_t pid)
{
	pid_t	*tmp;

	tmp = ft_calloc(1, sizeof(*tmp));
	*tmp = pid;
	return (tmp);
}

void	del_pid(void *content)
{
	free(content);
}

/** Waits every pids, and set last process pid's wstatus to config->last_cmd_status */
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
		cur = cur->next;
	}
	g_is_sig_interupt = false;
	ft_lstclear(&config->pid_list, del_pid);
}
