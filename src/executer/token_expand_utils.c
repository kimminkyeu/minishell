/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 19:20:29 by han-yeseul        #+#    #+#             */
/*   Updated: 2022/08/01 15:26:20 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand.h"
#include "executer.h"

extern int	g_is_sig_interupt;

char	**get_cmd_argv(t_list *token)
{
	t_token		*tok;
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
		tok = cur->content;
		arglist[word_cnt] = ft_strdup(tok->str->text);
		word_cnt++;
		cur = cur->next;
	}
	return (arglist);
}

// NOTE : if g_is_sig_interupt == true, signal: 2 + 128
int	expand_dollar_sign(t_string *str, t_iterator *iter, \
		t_shell_config *config, t_token_type token_type)
{
	long	start;
	long	end;
	char	*env_key;
	char	*env_value;
	int		status;

	status = 0;
	if (iter->f_peek(iter) == '?')
	{
		if (g_is_sig_interupt == true)
			str->f_append(str, "130");
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
		if (token_type == E_TYPE_REDIR_ARG)
		{
			if (env_value == NULL || ft_strchr(env_value, ' ') != NULL)
				status = 1;
		}
		free(env_key);
	}
	return (status);
}

/** NOTE : interpret $ARG */
int	expand_double_quote(t_string *str, t_iterator *iter, \
		bool *is_dollar_expanded, t_shell_config *config)
{
	char	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_next(iter);
		if (c == '\"')
			return (SUCCESS);
		else if (c == '$')
		{
			*is_dollar_expanded = true;
			expand_dollar_sign(str, iter, config, E_TYPE_DEFAULT);
		}		
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
