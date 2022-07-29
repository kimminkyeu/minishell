/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 23:56:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 16:36:39 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include "../../include/libft.h"

# define READ	(0)
# define WRITE	(1)

typedef struct s_shell_config {
	char	***envp;
	int		stdin_backup;
	int		stdout_backup;
	pid_t	last_cmd_pid;
	int		last_cmd_wstatus;
	t_list	*pid_list;
}	t_shell_config;

void	load_shell_config(t_shell_config *shell_config, char **env);
int		run_shell(char *line, t_shell_config *config);
void	shell_loop(t_shell_config *config);
void	show_shell_logo(void);
void	sig_ctrl_c(int signal);
void	set_signal(void);
char	*readline_prompt(t_shell_config *config);

#endif /* MINISHELL_H */
