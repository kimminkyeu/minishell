/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 23:56:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 01:51:47 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


typedef struct s_shell_config {

	char	***envp; // for environ functions

	int		stdin_backup;
	int		stdout_backup;

	int		pipe_fd[2];
	
	pid_t	last_cmd_pid; // for waitpid()
	int		last_cmd_wstatus;

} t_shell_config;

#endif /* MINISHELL_H */
