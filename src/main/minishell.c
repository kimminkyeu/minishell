/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/17 19:53:07 by minkyeki         ###   ########.fr       */
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


/** (2) helper functions. delete later */
#include "helper.h"

/** int	execute(t_tree *syntax_tree)
  * {
  *     (void)syntax_tree;
  *
  *     return (1);
  * } */

void	shell_loop(void)
{
	int		status;
	char	*line;
	t_list	*tokens;
	t_tree	*syntax_tree;

	status = 1;
	while (status)
	{
		/* Readline library  */
		line = readline("& ");
		add_history(line);

		/* (0) Check input. 쉘 입력값 검사(공백만 입력, 아무것도 없는 입력)
		 * ctrl+c : ^C가 메시지에 출력 + 프롬프트 새로 띄우기 
		 * ctrl+d : exit이 메시지에 출력 + 종료 */
		// ...


		/* (1) ------------------------------------- 
		 *     | Lexer + Syntax error check        | 
		 *     -------------------------------------*/
		tokens = tokenize(line);
		if (tokens == NULL) /* NULL if syntax error */
		{
			free(line);
			continue ;
		}
		/** Visualize token_list */
		print_tokens(tokens);		



		/* (2) ------------------------ 
		 *     | Parser (AST)         | 
		 *     ------------------------*/
		syntax_tree = parse(tokens);
		print_tree(syntax_tree);



		/* (3) Executer + execution error checking
		 *
		 * NOTE : 모든 문법 검사는 각 토큰 실행 직전에 한다.
		 * */
		/** status = execute(syntax_tree); */
		/** NOTE : 
		  * token_list를 편집해서 tree로 만들었기 때문에, 실행하는 executer에서
		  * 실행 후 free해주는 (자원 정리) 과정을 책임져야 한다. */



		free(line);
	}
}

/* NOTE : https://brennan.io/2015/01/16/write-a-shell-in-c/ */

int main(int ac, char **av, char **env)
{	
	(void)ac;
	(void)av;

	/* (1) Load config files, Environ, set data etc... */
	(void)env;

	/* (2) Run command loop */
	shell_loop();

	/* (3) Perform any shutdown/cleanup  */

	return (EXIT_SUCCESS);
}
