/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 15:44:57 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/26 14:55:41 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand.h"
#include "executer.h"

char	**get_cmd_argv(t_list *token)
{
	t_string	*joined_str;
	t_list		*cur;
	char		**arglist;

	joined_str = new_string(50);
	cur = token;
	while (cur != NULL)
	{
		t_token *tok = cur->content;
		joined_str->f_append(joined_str, tok->str->text);
		joined_str->f_push_back(joined_str, ' ');
		cur = cur->next;
	}
	arglist = ft_split(joined_str->text, ' ');
	delete_string(&joined_str);
	return (arglist);
}

void	expand_dollar_sign(t_string *str, t_iterator *iter, t_shell_config *config)
{
	long	start;
	long	end;
	char	*env_key;
	char	*env_value;

	/** $? 처리.  */
	if (iter->f_peek(iter) == '?')
	{
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

int	expand_tokens_redirection(t_list *tokens, t_shell_config *config)
{
	t_list	*cur;
	t_token *tok;
	int		status;

	cur = tokens;
	status = SUCCESS;
	while (cur != NULL && status == SUCCESS)
	{
		tok = cur->content;
		if (tok->type == E_TYPE_REDIR_LESS || tok->type == E_TYPE_REDIR_GREATER
			|| tok->type == E_TYPE_REDIR_APPEND)
		status = expand_token_each(tok, config);
		cur = cur->next;
	}
	return (status);
}
