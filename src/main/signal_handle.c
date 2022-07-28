/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 01:57:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 13:40:14 by han-yeseul       ###   ########.fr       */
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
	/** FIXME :
	 * (1) cat 혹은 sleep 10만 쳤을 때, Ctrl+C를 누르면 프롬프트가 두번 뜸...
	 * 대체 왜일까?
	 * */


	if (signal == SIGINT)
	{
		g_is_sig_interupt = true;
		// rl_replace_line("", 1);
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_redisplay();
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
			wait(NULL);
		if (g_is_sig_interupt == true)
		{
			cur = config->pid_list;
			while (cur != NULL)
			{
				cur_pid = *((pid_t *)cur->content);
				kill(cur_pid, SIGTERM);
				cur = cur->next;
			}
			g_is_sig_interupt = false;
			break ;
		}
		cur = cur->next;
	}
	g_is_sig_interupt = false;
	ft_lstclear(&config->pid_list, del_pid);
}
