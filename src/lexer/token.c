/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:03:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 14:24:02 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

void	token_reset(t_token *token)
{
	t_string	*string;
	
	string = token->str;
	string->f_clear(string);
	token->type = E_TYPE_DEFAULT;
}

void	token_push_back(t_token *token, char c)
{
	t_string	*string;

	string = token->str;
	string->f_push_back(string, c);
}

void	token_pop_back(t_token *token)
{
	t_string	*string;

	string = token->str;
	string->f_pop_back(string);
}

void	token_append(t_token *token, const char *str_in)
{
	t_string	*string;

	string = token->str;
	string->f_append(string, str_in);
}

void	delete_token(void *_token)
{
	t_token *token;

	token = _token;
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
	token->str->f_append(token->str, str_input);
	token->f_append = token_append;
	token->f_pop_back = token_pop_back;
	token->f_push_back = token_push_back;
	token->f_reset = token_reset;
	return (token);
}
