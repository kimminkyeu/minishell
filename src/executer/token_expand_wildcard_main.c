/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard_main.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 00:44:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 15:18:30 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand_wildcard.h"

t_list	*expand_single_wildcard(t_token *tok, t_shell_config *config)
{
	t_pattern	pattern;
	t_list		*new;
	t_list		*cur;
	t_token		*tok2;

	init_pattern(&pattern);
	set_pattern(tok, &pattern, config);
	new = match_path_and_return_list(pattern.path->text, &pattern);
	if (pattern.path->text[pattern.path->text_len - 1] != '/')
		pattern.path->f_push_back(pattern.path, '|');
	cur = new;
	while (cur != NULL)
	{
		tok2 = cur->content;
		tok2->str->f_insert(tok2->str, 0, \
				ft_strrchr(pattern.path->text, '/') + 1);
		tok2->str->f_replace_all(tok2->str, "|", "/");
		tok2->str->f_append(tok2->str, pattern.remainder->text);
		cur = cur->next;
	}
	delete_pattern(&pattern);
	return (new);
}

t_list	*expand_wildcard_glob_and_return_list(t_list *cur_token, \
		t_shell_config *config, int *is_error)
{
	t_token	*tok;
	t_list	*expanded_token;
	char	*redir_err_messege;

	if (cur_token == NULL)
		return (NULL);
	expanded_token = NULL;
	tok = cur_token->content;
	redir_err_messege = ft_strdup(tok->str->text);
	expanded_token = expand_single_wildcard(tok, config);
	if ((tok->type >= 5 && tok->type <= 9) || tok->type == 13)
	{
		if (expanded_token != NULL && expanded_token->next != NULL)
		{
			printf("lesh: %s: ambiguous redirect\n", redir_err_messege);
			ft_lstclear(&expanded_token, delete_token);
			expanded_token = NULL;
			*is_error = true;
		}
	}
	if (redir_err_messege != NULL)
		free(redir_err_messege);
	return (expanded_token);
}

int	expand_wildcard_glob_once(t_list *cur, \
		t_shell_config *config, int *is_error)
{
	t_list	*tmp;
	t_list	*expanded_list;
	t_list	*prev;

	while (cur != NULL && cur->next != NULL)
	{
		if (has_wild_card(cur->next))
		{
			tmp = cur->next->next;
			prev = cur->next;
			expanded_list = expand_wildcard_glob_and_return_list(\
					cur->next, config, is_error);
			if (expanded_list != NULL)
			{
				cur->next = expanded_list;
				while (cur != NULL && cur->next != NULL)
					cur = cur->next;
			}
			cur->next = tmp;
			ft_lstdelone(prev, delete_token);
		}
		cur = cur->next;
	}
	return (*is_error);
}

int	expand_wildcard_glob(t_list *tokens, t_shell_config *config)
{
	int		status;
	int		has_wild_card;
	t_token	*tok;
	char	*before_expansion;
	int		is_error;

	tok = NULL;
	is_error = false;
	before_expansion = NULL;
	if (tokens != NULL && tokens->next != NULL)
	{
		tok = tokens->next->content;
		before_expansion = ft_strdup(tok->str->text);
	}
	status = SUCCESS;
	has_wild_card = false;
	while (tokens != NULL && tokens->next != NULL \
			&& status == SUCCESS && has_wild_card_in_list(tokens))
	{
		has_wild_card = true;
		status = expand_wildcard_glob_once(tokens, config, &is_error);
	}
	if (has_wild_card == true && tokens->next == NULL)
		tokens->next = ft_lstnew(new_token(before_expansion));
	return (status);
}
