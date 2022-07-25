/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 23:56:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 18:57:04 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


typedef struct s_shell_config {

	char	***envp; // for environ functions

	int		stdin_backup;
	int		stdout_backup;

	int		pipefd_save;

	pid_t	last_cmd_pid; // for waitpid()
	int		last_cmd_wstatus;

} t_shell_config;

#endif /* MINISHELL_H */
