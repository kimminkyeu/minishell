/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/20 19:36:01 by minkyeki         ###   ########.fr       */
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

/** (2) helper functions for Visualization. 
 * TODO : Delete helper files later! */
#include "helper.h"

void	shell_loop(void) /* TODO : change void type to t_minishell */
{
	int		status;
	char	*line;
	t_list	*tokens;
	t_tree	*syntax_tree;

	status = CMD_SUCCEESS;
	while (status != CMD_STOP_SHELL)
	{
		/* Readline library  */
		line = readline("\033[31mlesh& \033[0m");
		add_history(line);

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

		status = execute(syntax_tree);
		free(line);
		/** system("leaks minishell > leaks_result_temp; cat leaks_result_temp | grep leaked && rm -rf leaks_result_temp"); */
	}
}

int main(int ac, char **av, char **env)
{	
	(void)ac;
	(void)av;

	/* (1) Load config files, Environ, set data etc... */
	(void)env;

	/* (2) Run command loop */
	shell_loop( /*  TODO : pass shell data here... */ );

	/* (3) Perform any shutdown/cleanup  */
	// ...
	return (EXIT_SUCCESS);
}
