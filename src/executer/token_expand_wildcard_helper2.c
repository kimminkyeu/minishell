/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard_helper2.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 21:12:45 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 15:16:20 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand_wildcard.h"

int	has_wild_card(t_list *cur_token)
{
	t_token	*tok;

	if (cur_token == NULL)
		return (false);
	tok = cur_token->content;
	if (ft_strchr(tok->str->text, '*'))
		return (true);
	else
		return (false);
}

int	has_wild_card_in_list(t_list *tokens)
{
	t_list	*cur;

	cur = tokens;
	while (cur != NULL)
	{
		if (has_wild_card(cur))
			return (true);
		cur = cur->next;
	}
	return (false);
}

int	ft_strncmp_reverse(const char *s1, const char *s2, size_t n)
{
	char	*s1_uchar;
	char	*s2_uchar;

	s1_uchar = ft_strrchr(s1, '\0');
	s2_uchar = ft_strrchr(s2, '\0');
	while (s1_uchar >= s1 && s2_uchar >= s2 \
			&& (*s1_uchar == *s2_uchar) && n > 0)
	{
		s1_uchar--;
		s2_uchar--;
		n--;
	}
	return ((int)(*s1_uchar - *s2_uchar));
}
