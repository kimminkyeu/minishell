/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/15 23:06:10 by minkyeki         ###   ########.fr       */
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


/** (2) helper functions. delete later */
#include "helper.h"

typedef enum e_cmd_type {
	E_DEFAULT, /* NOTE : for recursive step */
	E_SIMPLE_CMD,
	E_SUB_SHELL,
	E_PIPE,
	E_REDIR,
	/* ... add type here. */
} t_cmd_type;

typedef struct s_tree	t_tree;

typedef struct s_tree {
	t_cmd_type	type;
	t_list		*token;
	t_tree		*left;
	t_tree		*right;
} t_tree;

t_tree *parse(t_list *token_list)
{
	t_tree *tree;

	(void)token_list;

	return (tree);
}

int	execute(t_tree *syntax_tree)
{
	(void)syntax_tree;

	return (1);
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

		/* (1) Lexer */
		tokens = tokenize(line);
		print_tokens(tokens);

		/* (2) Parser */
		syntax_tree = parse(tokens);

		/* (3) Executer */
		status = execute(syntax_tree);


		/** (4) free token list and line... etc */
		ft_lstclear(&tokens, delete_token); /* NOTE : delete this function!!! */
		// TODO : 
		// token_list를 편집해서 tree로 만들었기 때문에, 실행하는 executer에서 
		// 실행 후 free해주는 (자원 정리) 과정을 책임져야 한다.
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
