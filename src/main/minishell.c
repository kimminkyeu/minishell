/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 16:44:54 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

/** Our Source header */
/* (1) Lexer header */
#include "minishell.h"
#include "heredoc.h"

#include "../../include/execute.h"
#include "../lexer/token.h"
/** (2) helper functions for Visualization.
 * TODO : Delete helper files later! */
#include "helper.h"

/* run shell에 추가된 set heredoc에서 limiter의 쿼트리무벌 한 다음에 
 * 파이프 열어서 파이프에 쓰고 파이프의 fd를 토큰에 저장하고 1차 종료 */

/** 실행하기 전에 set redirection할 때 heredoc이면 
 * open 대신에 이미 열려있는 pipefd[read]를 가져와서 읽어준다. */

/** 이런 식으로 하려고 하는데, 파일에서 파이프로 바꿨다고만 생각했는데, 실행이 이상한데 왜때문인지 모르겠어요. 아무것도 안 되긴 하는데, 실행해서 ls <<LIMIT 해보시면 입력창이 이상해요. */
int	run_shell(char *line, t_shell_config *config)
{
	t_list	*tokens;
	t_tree	*syntax_tree;

	tokens = tokenize(line);//syntax error check done
	if (tokens == NULL)
	{
		free(line);
		return (CMD_FAILURE);
	}
	print_tokens(tokens); // TODO : delete later

	set_heredoc(tokens);//quote removal + open temp file
						//
	print_tokens(tokens); // TODO : delete later

	syntax_tree = parse(tokens);
	if (syntax_tree == NULL)
	{
		free(line);
		free(tokens);
		return (CMD_FAILURE);
	}
	print_tree(syntax_tree); // TODO : delete later

	(void)config;
	/** return (execute(syntax_tree, config)); */
	return (0);
}

void	shell_loop(t_shell_config *config)
{
	int		status;
	char	*line;

	status = CMD_SUCCEESS;
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
		else
		{
			free(line);
			line = NULL;
			continue ;
		}
		status = run_shell(line, config);
		free(line);
		line = NULL;
	}
	/** system("leaks minishell > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp"); */
}

int main(int ac, char **av, char **env)
{
	t_shell_config	shell_config;

	(void)ac;
	(void)av;
	/* (1) Load config files, Environ, set data etc... */
	shell_config.envp = ft_calloc(1, sizeof(size_t));
	*shell_config.envp = new_environ(env); // load envp
	shell_config.stdin_backup = dup(STDIN_FILENO); // save STDIN
	shell_config.stdout_backup = dup(STDOUT_FILENO); // save STDOUT
	shell_config.last_cmd_pid = 0;
	shell_config.last_cmd_wstatus = 0;
	/** shell_config.pipe_fd[READ] = STDIN_FILENO; */
	/** shell_config.pipe_fd[WRITE] = STDOUT_FILENO; */
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
