/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/17 20:29:00 by minkyeki         ###   ########.fr       */
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


extern void	print_tree_node(t_list *token);

int	execute_node(t_tree *node)
{
	/** 아래 코드는 방문 순서를 확인하기 위한 코드일 뿐, 구현시엔 제거할 것. */
	print_tree_node(node->token);
	printf("\n");
	print_tree_node(node->redirection);
	printf("\n");
	printf("-------------------");
	printf("\n\n");


	/** NOTE : write execution code here */




	return (1);
}


void	inorder_recur(t_tree *node, int *status)
{
	/** status가 몇일 때 어떤 행동을 할지는 구현할 때 정하기 */
	if (node == NULL || *status == 0)
		return ;
	inorder_recur(node->left, status);
	*status = execute_node(node);
	inorder_recur(node->right, status);
}

/** Executer function. make traversing */
int	execute(t_tree *syntax_tree)
{
	int	status;


	printf("\n");
	printf("--------------------------------\n");
	printf("|     Tokenizer Result         |\n");
	printf("--------------------------------\n");

	status = 1;
	inorder_recur(syntax_tree, &status);

	return (status);
}

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


		/* (3) -------------------------------------- 
		 *     | Executer (inorder traverse)        | 
		 *     --------------------------------------*/
		status = execute(syntax_tree);

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
