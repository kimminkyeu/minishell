/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 22:20:22 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/15 22:23:59 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include "helper.h"
# include "../../include/lexer.h"

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

void	print_tokens(t_list *tokens)
{
	t_list	*tmp;

	tmp = tokens;
	while (tmp != NULL)
	{
		t_token *tok = tmp->content;
		printf("\033[93m%20s $\033[0m", get_token_type(tok->type));
		printf("%s\n", tok->str->text);
		tmp = tmp->next;
	}
}
