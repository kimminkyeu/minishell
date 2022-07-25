/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 23:56:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 22:06:40 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>

# define READ	(0)
# define WRITE	(1)

typedef struct s_shell_config {
	char	***envp;
	int		stdin_backup;
	int		stdout_backup;
	int		pipe_fd[2];
	pid_t	last_cmd_pid;
	int		last_cmd_wstatus;
} t_shell_config;

int		run_shell(char *line, t_shell_config *config);
void	shell_loop(t_shell_config *config);
void	show_shell_logo(void);
void	sig_ctrl_c(int signal);
void	set_signal(void);
char	*readline_prompt(t_shell_config *config);

#endif /* MINISHELL_H */
