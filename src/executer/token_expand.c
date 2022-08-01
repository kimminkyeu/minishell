/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:44:57 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/01 19:44:10 by minkyeki         ###   ########.fr       */
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

/** NOTE : is_dollar_expanded Flag is for [split_via_whitespace()] */
int	expand_token_each(t_token *tok, bool *is_dollar_expanded, \
		t_shell_config *config)
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
			status = expand_double_quote(expanded_str, &iter, \
					is_dollar_expanded, config);
		else if (c == '$')
			expand_dollar_sign(expanded_str, &iter, is_dollar_expanded, config);
		else
			expanded_str->f_push_back(expanded_str, c);
	}
	delete_string(&tok->str);
	tok->str = expanded_str;
	return (status);
}

/** Before : [ec$TEST]
 *  Mid    : [echo hi hello]-[NULL]     --> Dollar sign is expanded.
 *  After  : [echo]-[hi]-[hello]-[NULL] --> split via ' '
 **/
void	split_via_whitespace(t_list *tokens)
{
	t_list	*cur;
	t_token	*tok;
	t_list	*tmp;
	char	*spliter;

	cur = tokens;
	while (cur != NULL)
	{
		tok = cur->content;
		spliter = ft_strchr(tok->str->text, ' ');
		while (spliter != NULL)
		{
			*spliter = '\0';
			tmp = cur->next;
			cur->next = ft_lstnew(new_token(spliter + 1));
			cur->next->next = tmp;
			spliter = ft_strchr(tok->str->text, ' ');
		}
		cur = cur->next;
	}
}

/* NOTE : Iterator Pattern refactoring 
 * Because ec$ECHO becomes [echo hi]-[NULL],
 * additional splitting is needed (ex. echo - hi - null)
 * */
int	expand_tokens(t_list *tokens, t_shell_config *config)
{
	t_list	*cur;
	t_token	*tok;
	int		status;
	bool	is_dollar_expanded;

	cur = tokens;
	status = SUCCESS;
	is_dollar_expanded = false;
	while (cur != NULL && status == SUCCESS)
	{
		tok = cur->content;
		status = expand_token_each(tok, &is_dollar_expanded, config);
		cur = cur->next;
	}
	if (status == SUCCESS && is_dollar_expanded == true)
		split_via_whitespace(tokens);
	if (status == SUCCESS)
		status = expand_wildcard_glob(tokens, config);
	return (status);
}
