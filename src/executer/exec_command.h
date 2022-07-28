/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 17:38:29 by han-yeseul        #+#    #+#             */
/*   Updated: 2022/07/28 17:38:55 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_COMMAND_H
# define EXEC_COMMAND_H

/*    exec_command.c    */
void	exec_priority_operator(t_tree *node, t_token *tok, int *status, \
			t_shell_config *config);
int		exec_general(t_tree *node, char **cmd_argv, t_shell_config *config);
int		exec_exceptions(t_tree *node, char **cmd_argv, t_shell_config *config);
int		exec_builtin(char **cmd_argv, char ***envp);

/*    exec_command_utils.c    */
//Function for exec_general
void	parent_process(int *pipe_fd, pid_t pid, t_tree *node, \
			t_shell_config *config);
void	child_process(int *pipe_fd, t_tree *node, char **cmd_argv, \
			t_shell_config *config);
void	exec_command(char **cmd_argv, t_shell_config *config);

//others
int		is_builtin_func(char *cmd);
int		is_cd_or_exit_or_export(char *cmd);

#endif/* EXEC_COMMAND_H */
