/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 15:25:24 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

/** Our Source header */
/* (1) Lexer header */
#include "minishell.h"

#include "../../include/execute.h"
/** (2) helper functions for Visualization. 
 * TODO : Delete helper files later! */
#include "helper.h"

int	run_shell(char *line, t_shell_config *config)
{
	t_list	*tokens;
	t_tree	*syntax_tree;

	tokens = tokenize(line);
	if (tokens == NULL)
		return (CMD_KEEP_RUNNING);
	print_tokens(tokens); // TODO : delete later
	syntax_tree = parse(tokens);
	if (syntax_tree == NULL)
	{
		free(tokens);
		return (CMD_KEEP_RUNNING);
	}
	print_tree(syntax_tree); // TODO : delete later
	return (execute(syntax_tree, config));
}

void	shell_loop(t_shell_config *config)
{
	int		status;
	char	*line;

	status = CMD_KEEP_RUNNING;

	/** FIXME : CMD_STOP_SHELL 이 필요한가? 
	 *          필요 없다면 그냥 while(true)로 변경할 것.
	 **/
	while (status != CMD_STOP_SHELL)
	{
		line = readline_prompt(config);
		if (line == NULL)
		{
			printf("exit\n");
			exit(-1);
		}
		else if (line != NULL && *line != '\0')
			add_history(line);
		status = run_shell(line, config);
		free(line);
		line = NULL;
	}
	/** system("leaks minishell > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp"); */
}

void	load_shell_config(t_shell_config *shell_config, char **env)
{
	shell_config->envp = ft_calloc(1, sizeof(size_t));
	*shell_config->envp = new_environ(env); // load envp
	shell_config->stdin_backup = dup(STDIN_FILENO); // save STDIN
	shell_config->stdout_backup = dup(STDOUT_FILENO); // save STDOUT
	shell_config->last_cmd_pid = 0;
	shell_config->last_cmd_wstatus = 0;
}

int main(int ac, char **av, char **env)
{	
	t_shell_config	shell_config;

	(void)ac;
	(void)av;

	/* (1) Load config files, Environ, set data etc... */
	load_shell_config(&shell_config, env);

	/* (2) Set signal for Ctrl+C | Ctrl+\ */
	set_signal();

	/* (+) Show Lee-Shell Logo */
	show_shell_logo();

	/* (3) Run command loop */
	shell_loop(&shell_config);

	/* (4) Perform any shutdown/cleanup  */
	delete_environ(shell_config.envp);
	free(shell_config.envp);

	return (EXIT_SUCCESS);
}
