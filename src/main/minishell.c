/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 19:05:25 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Standard library */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

/** Our Source header */
/* (1) Lexer header */
#include "../../include/lexer.h"
#include "../../include/parse.h"
#include "../../include/execute.h"
#include "../../include/builtin.h"
#include "minishell.h"

/** (2) helper functions for Visualization.
 * TODO : Delete helper files later! */
#include "helper.h"

#define READ	(0)
#define WRITE	(1)

/** NOTE
 * : 시그널 부분 참고 https://velog.io/@sham/minishell%EA%B3%BC-readline */

void	sig_ctrl_c(int signal)
{
	(void)signal;

	rl_on_new_line(); // 개행 삽입.
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_redisplay();
}

void	set_signal(void)
{
	signal(SIGINT, sig_ctrl_c); // CTRL + C
	signal(SIGQUIT, SIG_IGN); // CTRL + / -> SIG_IGN = signal 무시.
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

void	shell_loop(t_shell_config *config)
{
	int		status;
	t_list	*tokens;
	t_tree	*syntax_tree;
	char	*line;

	status = CMD_SUCCEESS;
	while (status != CMD_STOP_SHELL)
	{
		/* Readline library  */
		line = readline_prompt(config);

		/** CTRL + D 를 눌렀을 때, realine에서 NULL문자를 반환한다. */
		if (line == NULL)
		{
			printf("exit\n");
			exit(-1);
		}
		if (line != NULL && *line != '\0')
			add_history(line);
		else
		{
			free(line);
			continue ;
		}


		/* (0) Check input. 쉘 입력값 검사(공백만 입력, 아무것도 없는 입력)
		 * ctrl+c : ^C가 메시지에 출력 + 프롬프트 새로 띄우기
		 * ctrl+d : exit이 메시지에 출력 + 종료 */

		// ...


		/* (1) -------------------------------------
		 *     |  Lexer + Syntax error check       |
		 *     ------------------------------------*/
		tokens = tokenize(line);
		if (tokens == NULL) /* NULL if syntax error occurs */
		{
			free(line);
			status = CMD_FAILURE;
			continue ;
		}
		else /* Visualize tokens */
			print_tokens(tokens);


		/* (2) ------------------------
		 *     |  Parser (AST)        |
		 *     ------------------------*/
		syntax_tree = parse(tokens);
		if (syntax_tree == NULL)
		{
			free(line);
			free(tokens);
			status = CMD_FAILURE;
			continue ;
		}
		else
			print_tree(syntax_tree);


		/* (3) ------------------------------------------
		 *     |  Executer (inorder traverse)           |
		 *     |----------------------------------------|
		 *     |  (1) node free를 여기서 수행한다.      |
		 *     |                                        |
		 *     |  (2) NOTE : 노드 방문중 에러 발생시    |
		 *     |	  모든 노드를 해제해야 한다.        |
		 *     -----------------------------------------*/

		status = execute(syntax_tree, config);
		if (line != NULL)
		{
			free(line);
			line = NULL;
		}
		/** system("leaks minishell > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp"); */
	}
}


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


int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_shell_config	shell_config;

	/* (1) Load config files, Environ, set data etc... */
	shell_config.envp = ft_calloc(1, sizeof(size_t));
	*shell_config.envp = new_environ(env); // load envp
	shell_config.stdin_backup = dup(STDIN_FILENO); // save STDIN
	shell_config.stdout_backup = dup(STDOUT_FILENO); // save STDOUT
	shell_config.last_cmd_pid = 0;
	shell_config.last_cmd_wstatus = 0;
	shell_config.pipefd_save = shell_config.stdin_backup;

	/* 리눅스에선 이거 세팅 안해도 되는 데?  */
	set_signal();

	/* (+) Show Lee-Shell Logo */
	show_shell_logo();


	/* (2) Run command loop */
	shell_loop(&shell_config);


	/* (3) Perform any shutdown/cleanup  */
	delete_environ(shell_config.envp);
	free(shell_config.envp);

	return (EXIT_SUCCESS);
}
