/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:23:40 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 23:14:18 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iterator.h"

void	init_iterator(t_iterator *iter, char *line)
{
	iter->line = line;
	iter->line_len = ft_strlen(line);
	iter->curpos = INIT_SRC_POS;
	iter->f_next = iter_next;
	iter->f_peek = iter_peek;
	iter->f_skip_white_space = iter_skip_white_space;
	iter->f_unget = iter_unget;
	iter->f_has_next = iter_has_next;
}

/** 다음 문자가 있는지, eof는 아닌지 체크 */
int	iter_has_next(t_iterator *iter)
{
	char next_char;
	
	next_char = iter->f_peek(iter);
	if (next_char != '\n' && next_char != '\0' && next_char != EOF)
		return (1);
	else
		return (0);
}

/* move iterator to point next character */
char iter_next(t_iterator *iter)
{
	char c1;

	c1 = '\0';
	if (!iter || !(iter->line))
	{
		errno = ENODATA;
		return (ERRCHAR);
	}
	if (iter->curpos == INIT_SRC_POS)
		iter->curpos = -1;
	/** printf("line len : %zd\n", iter->line_len); */
	if (++iter->curpos >= iter->line_len)
	{
		iter->curpos = iter->line_len;
		return (EOF);
	}
	c1 = iter->line[iter->curpos];
	/** printf("iterator next :%c, curpos:%ld\n", c1, iter->curpos); */
	return (c1);
}

void iter_unget(t_iterator *iter)
{
	if (iter->curpos < 0)
		return;
	iter->curpos--;
}

/** NOTE : cursur_pos를 움직이지 않고 다음 문자만 확인. */
char iter_peek(t_iterator *iter)
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

/* move iterator to point next character */
void	iter_skip_white_space(t_iterator *iter)
{
	char	c;

	if (!iter || !iter->line)
		return ;
	/** 다음 문자가 EOF가 아니고 공백일 때 까지 */
	while (((c = iter->f_peek(iter)) != EOF) && (ft_isspace(c)))
		iter->f_next(iter);
}
