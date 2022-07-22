/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 23:56:07 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 23:57:19 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef union u_pipe {
	struct {
		int	read;
		int	write;
	};
	int	fd[2];
}	t_pipe;

typedef struct s_shell_config {

	char	**envp; // for environ functions

	int		stdin_backup;
	int		stdout_backup;
	
	pid_t	last_cmd_pid; // for waitpid()

	t_pipe	pipe;  // for pipe()

} t_shell_config;

#endif /* MINISHELL_H */
