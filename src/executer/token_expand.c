/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:44:57 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 19:21:57 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand.h"
#include "executer.h"

/** NOTE : spliting with ' ' (aka. whitespace) is a problem.
 *         For example [ grep "my name" ] becomes [grep, my, name].
 *         As a result [grep] only finds word [my].
 *         Inorder to fix this problem, I changed "ft_split" logic.
 **/

extern int	g_is_sig_interupt;

int	expand_token_each(t_token *tok, t_shell_config *config)
{
	int			status;
	t_iterator	iter;
	t_string	*expanded_str;
	char		c;

	status = SUCCESS;
	init_iterator(&iter, tok->str->text);
	expanded_str = new_string(tok->str->capacity);
	while (iter.f_has_next(&iter) && status == SUCCESS)
	{
		c = iter.f_next(&iter);
		if (c == '\'')
			status = expand_single_quote(expanded_str, &iter);
		else if (c == '\"')
			status = expand_double_quote(expanded_str, &iter, config);
		else if (c == '$')
			expand_dollar_sign(expanded_str, &iter, config);
		else
			expanded_str->f_push_back(expanded_str, c);
	}
	delete_string(&tok->str);
	tok->str = expanded_str;
	return (status);
}

/* NOTE : Iterator Pattern refactoring */
int	expand_tokens(t_list *tokens, t_shell_config *config)
{
	t_list	*cur;
	t_token	*tok;
	int		status;

	cur = tokens;
	status = SUCCESS;
	while (cur != NULL && status == SUCCESS)
	{
		tok = cur->content;
		status = expand_token_each(tok, config);
		cur = cur->next;
	}
	return (status);
}
