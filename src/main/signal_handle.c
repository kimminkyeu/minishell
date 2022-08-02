/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 01:57:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 13:13:33 by minkyeki         ###   ########.fr       */
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
			/** TODO : Delete here */
			/** printf("g_is_sigint : %d\n", g_is_sig_interupt); */

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

void	del_pid(void *content)
{
	free(content);
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
		/** NOTE : signal ctrl+c 부분이 없음. */
		if (g_is_sig_interupt == true)
		{
			cur = config->pid_list;
			while (cur != NULL)
			{
				/** NOTE : Remove here */
				/** printf("killing child processes ...\n"); */
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

/** Waits every pids, and set last process pid's 
 *  wstatus to config->last_cmd_status. */
/** void	wait_every_pid(t_shell_config *config)
  * {
  *     t_list	*cur;
  *     pid_t	cur_pid;
  *
  *     cur = config->pid_list;
  *     [> NOTE : signal ctrl+c 부분이 없음. <]
  *
  *     while (cur != NULL)
  *     {
  *         cur_pid = *((pid_t *)cur->content);
  *         if (cur_pid == config->last_cmd_pid)
  *             waitpid(cur_pid, &config->last_cmd_wstatus, 0);
  *         else
  *             waitpid(cur_pid, NULL, 0);
  *         cur = cur->next;
  *     }
  *     g_is_sig_interupt = false;
  *     ft_lstclear(&config->pid_list, del_pid);
  * } */
