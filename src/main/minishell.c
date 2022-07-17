/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/17 17:41:34 by minkyeki         ###   ########.fr       */
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
		 * ctrl+d : exit이 메시지에 출력 + 종료
		 * */
		// ...


		/* (1) Lexer (Token list) */
		tokens = tokenize(line);
		print_tokens(tokens);	
		free(line);

		/* (2) Token을 한번 쭉 돌면서 1차 에러 체킹 (혹은 토큰 만들때 부터 검사 진행)
		 * ex. echo hello | grep hello >out | (a (c)b) 는 커맨드 파싱전에 끝난다.
		 * 검사 내용
		 * 1. 괄호안에 괄호가 있는가? --> 있다면 에러 처리. 트리를 만들지 않는다.
		 * 2. 
		 * */


		/* (3) Parser (AST) */
		syntax_tree = parse(tokens);
		print_tree(syntax_tree);



		/* (4) Executer + Check syntax of each node 
		 * NOTE : 모든 문법 검사는 각 토큰 실행 직전에 한다.
		 * */
		/** status = execute(syntax_tree); */


		/** NOTE : 
		  * token_list를 편집해서 tree로 만들었기 때문에, 실행하는 executer에서
		  * 실행 후 free해주는 (자원 정리) 과정을 책임져야 한다. */


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
