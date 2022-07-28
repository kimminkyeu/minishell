/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:44:57 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 03:25:25 by minkyeki         ###   ########.fr       */
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

char	**get_cmd_argv(t_list *token)
{
	t_list		*cur;
	char		**arglist;
	size_t		word_cnt;

	if (token == NULL)
		return (NULL);
	word_cnt = 0;
	cur = token;
	while (cur != NULL)
	{
		word_cnt++;
		cur = cur->next;
	}
	arglist = ft_calloc(word_cnt + 1, sizeof(*arglist));
	cur = token;
	word_cnt = 0;
	while (cur != NULL)
	{
		t_token *tok = cur->content;
		arglist[word_cnt] = ft_strdup(tok->str->text);
		word_cnt++;
		cur = cur->next;
	}
	return (arglist);
}

void	expand_dollar_sign(t_string *str, t_iterator *iter, t_shell_config *config)
{
	long	start;
	long	end;
	char	*env_key;
	char	*env_value;

	if (iter->f_peek(iter) == '?')
	{
		if (g_is_sig_interupt == true)
			str->f_append(str, "130"); // NOTE : signal 2 + 128 로 130이 된다.
		else
			str->f_append(str, ft_itoa(WEXITSTATUS(config->last_cmd_wstatus)));
		iter->f_next(iter);
	}
	else
	{
		start = iter->curpos;
		while (iter->f_has_next(iter) && ft_isalnum(iter->f_peek(iter)))
			iter->f_next(iter);
		end = iter->curpos;
		env_key = ft_substr(iter->line, start + 1, end - start);
		env_value = get_environ_value(env_key, *(config->envp));
		str->f_append(str, env_value);
		free(env_key);
	}
}

/** NOTE : interpret $ARG */
int	expand_double_quote(t_string *str, t_iterator *iter, t_shell_config *config)
{
	char	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_next(iter);
		if (c == '\"')
			return (SUCCESS);
		else if (c == '$')
			expand_dollar_sign(str, iter, config);
		else
			str->f_push_back(str, c);
	}
	iter->f_unget(iter);
	c = iter->f_peek(iter);
	print_error("lesh: syntax error near unexpected token", &c);
	return (ERROR);
}

/** NOTE : do not interpret $ARG */
int	expand_single_quote(t_string *str, t_iterator *iter)
{
	char	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_next(iter);
		if (c == '\'')
			return (SUCCESS);
		else
			str->f_push_back(str, c);
	}
	iter->f_unget(iter);
	c = iter->f_peek(iter);
	print_error("lesh: syntax error near unexpected token", &c);
	return (ERROR);
}

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
	t_token *tok;
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
