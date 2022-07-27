/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 02:03:37 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 03:31:22 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLE_H
# define SIGNAL_HANDLE_H

# include "signal_handle.h"
# include "minishell.h"

void	*new_pid(pid_t pid);
void	del_pid(void *content);

/* Waits every pids in config->pid_list.
 * then saves wstatus of last_cmd_pid to config->last_cmd_wstatus */
void	wait_every_pid(t_shell_config *config);

#endif /* SIGNAL_HANDLE_H */
