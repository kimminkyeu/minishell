/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 21:55:15 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 03:51:47 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Standard library */
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

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

/** show prompt_messege + readline */
char	*readline_prompt(t_shell_config *config)
{
	t_string	*prompt;
	char		*line;

	line = NULL;
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
