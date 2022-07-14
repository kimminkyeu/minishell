/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 15:16:51 by minkyeki         ###   ########.fr       */
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
#include "../include/lexer.h"


/** TODO : move <include> to minishell.h */

const char	*get_token_type(t_token_type type);

int main(void)
{
	char	*line;
	t_list	*tokens;

	while (true)
	{
		line = readline("> ");
		add_history(line);

		tokens = tokenize(line);
		printf("input : %s\n", line);
		

		t_list	*tmp;
		tmp = tokens;
		/** (3) print token list */
		while (tmp != NULL)
		{
			t_token *tok = tmp->content;
			printf("[%s] ", get_token_type(tok->type));
			printf("%s\n", tok->str->text);
			tmp = tmp->next;
		}

		/** (4) free token list */
		ft_lstclear(&tokens, delete_token);
		free(line);
	}
	return (EXIT_SUCCESS);
}

const char	*get_token_type(t_token_type type)
{
	if (type == E_TYPE_DEFAULT)
		return ("?");
	else if (type == E_TYPE_CMD_OR_ARG)
		return ("CMD_OR_ARG");
	else if (type == E_TYPE_CMD_OPTION)
		return ("CMD_OPTION");
	else if (type == E_TYPE_SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == E_TYPE_DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	else if (type == E_TYPE_PIPE)
		return ("PIPE");
	else if (type == E_TYPE_REDIRECT)
		return ("REDIRECT");
	else if (type == E_TYPE_BRACKET)
		return ("BRACKET");
	else if (type == E_TYPE_SEMICOLON)
		return ("SEMICOLON");
	else if (type == E_TYPE_AMPERSAND)
		return ("AMPERSAND");
	else if (type == E_TYPE_DOUBLE_AMPERSAND)
		return ("DOUBLE_AMPERSAND");
	else if (type == E_TYPE_DOUBLE_PIPE)
		return ("DOUBLE_PIPE");
	else
		return ("\033[31mERROR\033[0m");
}
