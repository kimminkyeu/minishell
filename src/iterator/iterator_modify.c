/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_modify.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:23:40 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 15:32:18 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iterator.h"

int	iter_has_next(t_iterator *iter)
{
	char	next_char;

	next_char = iter->f_peek(iter);
	if (next_char != '\n' && next_char != '\0' && next_char != EOF)
		return (1);
	else
		return (0);
}

char	iter_next(t_iterator *iter)
{
	char	c1;

	c1 = '\0';
	if (!iter || !(iter->line))
	{
		errno = ENODATA;
		return (ERRCHAR);
	}
	if (iter->curpos == INIT_SRC_POS)
		iter->curpos = -1;
	if (++iter->curpos >= iter->line_len)
	{
		iter->curpos = iter->line_len;
		return (EOF);
	}
	c1 = iter->line[iter->curpos];
	return (c1);
}

void	iter_unget(t_iterator *iter)
{
	if (iter->curpos < 0)
		return ;
	iter->curpos--;
}

char	iter_peek(t_iterator *iter)
{
	long	pos;

	if (!iter || !iter->line)
	{
		errno = ENODATA;
		return (ERRCHAR);
	}
	pos = iter->curpos;
	if (pos == INIT_SRC_POS)
		pos++;
	pos++;
	if (pos >= iter->line_len)
		return (EOF);
	return (iter->line[pos]);
}

void	iter_skip_white_space(t_iterator *iter)
{
	char	c;

	if (!iter || !iter->line)
		return ;
	c = iter->f_peek(iter);
	while ((c != EOF) && (ft_isspace(c)))
	{
		iter->f_next(iter);
		c = iter->f_peek(iter);
	}
}
