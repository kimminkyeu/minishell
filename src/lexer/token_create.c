/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:59:12 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 15:24:01 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

t_token	*new_token(char *str_input)
{
	t_token	*token;

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
