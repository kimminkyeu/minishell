/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:03:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/13 15:50:36 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	token_reset(t_token *token)
{
	t_string	*string;
	
	string = token->str;
	string->clear(string);
	token->type = E_TYPE_DEFAULT;
}

void	token_push_back(t_token *token, char c)
{
	t_string	*string;

	string = token->str;
	string->push_back(string, c);
}

void	token_pop_back(t_token *token)
{
	t_string	*string;

	string = token->str;
	string->pop_back(string);
}

void	token_append(t_token *token, const char *str_in)
{
	t_string	*string;

	string = token->str;
	string->append(string, str_in);
}

void	delete_token(t_token *token)
{
	if (token->str != NULL)
		delete_string(&token->str);
	free(token);
}

t_token	*new_token(char *str_input)
{
	t_token *token;

	token = ft_calloc(1, sizeof(*token));
	token->type = E_TYPE_DEFAULT;
	token->str = new_string(8);
	if (token->str == NULL)
		return (NULL);
	token->str->append(token->str, str_input);
	token->append = token_append;
	token->pop_back = token_pop_back;
	token->push_back = token_push_back;
	token->reset = token_reset;
	return (token);
}
