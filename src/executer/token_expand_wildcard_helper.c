/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard_helper.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 21:10:00 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 15:15:58 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand_wildcard.h"

void	get_prefix(t_iterator *iter, t_string *prefix)
{
	int	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_peek(iter);
		if (c == '*')
			break ;
		else
			prefix->f_push_back(prefix, iter->f_next(iter));
	}
}

void	get_suffix(t_iterator *iter, t_string *suffix)
{
	int	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_peek(iter);
		if (c == '/')
			break ;
		else
			suffix->f_push_back(suffix, iter->f_next(iter));
	}
}

void	set_pattern(t_token *tok, t_pattern *pattern, t_shell_config *config)
{
	t_iterator	iter;

	init_iterator(&iter, tok->str->text);
	pattern->path->f_append(pattern->path, \
			get_environ_value("PWD", *config->envp));
	pattern->path->f_push_back(pattern->path, '/');
	set_pattern_path(tok, pattern, &iter);
	set_pattern_prefix_suffix_remainder(pattern, &iter);
}

void	init_pattern(t_pattern *pattern)
{
	pattern->path = new_string(32);
	pattern->prefix = new_string(32);
	pattern->suffix = new_string(32);
	pattern->remainder = new_string(32);
}

void	delete_pattern(t_pattern *pattern)
{
	delete_string(&pattern->path);
	delete_string(&pattern->prefix);
	delete_string(&pattern->suffix);
	delete_string(&pattern->remainder);
}
