/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 09:59:33 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 00:04:55 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "scanner.h"
#include "token.h"

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

/* Test for lexical analizer */
int	main(int ac, char **av)
{
	t_list	*tokens;

	if (ac != 2)
		return (EXIT_FAILURE);

	/** (1) get token list from tokenizer */
	tokens = tokenize(av[1]);


	/** (2) check token error (raw syntax cheking) */
	// ...


	
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
	
	return (0);
}
