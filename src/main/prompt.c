/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 21:55:15 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 22:06:08 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

/** Standard library */
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include "minishell.h"
#include "helper.h"
#include "../../include/builtin.h"

void	show_shell_logo(void)
{
	const char *red;
	const char *white;

	red = "\033[31m";
	white = "\033[0m";
	printf("%s===========================================================================%s\n", red, white);
	printf("%s|                                                                         |%s\n", red, white);
	printf("%s|   Welcome to 42 minishell project.                                      |%s\n", red, white);
	printf("%s|                                                                         |%s\n", red, white);
	printf("%s|                                                                         |%s\n", red, white);
	printf("%s|   ██╗     ███████╗███████╗    ███████╗██╗  ██╗███████╗██╗     ██╗       |%s\n", red, white);
	printf("%s|   ██║     ██╔════╝██╔════╝    ██╔════╝██║  ██║██╔════╝██║     ██║       |%s\n", red, white);
	printf("%s|   ██║     █████╗  █████╗      ███████╗███████║█████╗  ██║     ██║       |%s\n", red, white);
	printf("%s|   ██║     ██╔══╝  ██╔══╝      ╚════██║██╔══██║██╔══╝  ██║     ██║       |%s\n", red, white);
	printf("%s|   ███████╗███████╗███████╗    ███████║██║  ██║███████╗███████╗███████╗  |%s\n", red, white);
	printf("%s|   ╚══════╝╚══════╝╚══════╝    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝  |%s\n", red, white);
	printf("%s|                                                                         |%s\n", red, white);
	printf("%s|                                          .created by yehan & minkyeki   |%s\n", red, white);
	printf("%s|                                                                         |%s\n", red, white);
	printf("%s===========================================================================%s\n", red, white);
	printf("\n");
}

void	sig_ctrl_c(int signal)
{
	(void)signal;

	rl_on_new_line();
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_replace_line("", 1);
	rl_redisplay();
}

void	set_signal(void)
{
	signal(SIGINT, sig_ctrl_c);		// CTRL + C
	signal(SIGQUIT, SIG_IGN);		// CTRL + / -> SIG_IGN = signal 무시.
}

/** show prompt_messege + readline */
char	*readline_prompt(t_shell_config *config)
{
	t_string	*prompt;
	static char	*line;

	prompt = new_string(64);
	prompt->f_append(prompt, "\033[31m");
	prompt->f_append(prompt, get_environ_value("LOGNAME", *config->envp));
	prompt->f_append(prompt, "@");
	prompt->f_append(prompt, get_environ_value("NAME", *config->envp));
	prompt->f_append(prompt, ":");
	prompt->f_append(prompt, get_environ_value("PWD", *config->envp));
	prompt->f_replace_all(prompt, get_environ_value("HOME", *config->envp), "~");
	prompt->f_append(prompt, "\033[0m");
	prompt->f_append(prompt, "$ ");
	line = readline(prompt->text);
	delete_string(&prompt);
	return (line);
}
