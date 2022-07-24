/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/24 19:37:04 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Standard library */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

/** NOTE : 참고 https://velog.io/@sham/minishell%EA%B3%BC-readline */
void	shell_loop(t_shell_config *shell_config)
{
	int		status;
	t_list	*tokens;
	t_tree	*syntax_tree;
	static char	*line;

	status = CMD_SUCCEESS;
	while (status != CMD_STOP_SHELL)
	{
		printf("=============== reading next =============\n");


	/* Readline library  */
		line = readline("\033[31mlesh& \033[0m");

		/** FIXME : 오류. readline에서 자꾸 null을 반환해서 프로그램이 끝남... */
		if (line == NULL)
		{
			printf(" exit\n");
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

		status = execute(syntax_tree, shell_config);
		if (line != NULL)
		{
			free(line);
			line = NULL;
		}
		/** system("leaks minishell > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp"); */
	}
}

int main(int ac, char **av, char **env)
{	
	(void)ac;
	(void)av;
	t_shell_config	shell_config;

	/* (1) Load config files, Environ, set data etc... */
	shell_config.envp = new_environ(env); // load envp
	shell_config.stdin_backup = dup(STDIN_FILENO); // save STDIN
	shell_config.stdout_backup = dup(STDOUT_FILENO); // save STDOUT
	shell_config.last_cmd_pid = 0;


	/* (2) Run command loop */
	shell_loop(&shell_config);


	/* (3) Perform any shutdown/cleanup  */
	delete_environ(&shell_config.envp);

	return (EXIT_SUCCESS);
}
