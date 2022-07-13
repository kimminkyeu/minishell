/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:23:40 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 21:12:25 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "libft.h"
#include "iterator.h"

// typedef struct s_iterator {
// 	char	*buffer; 	/* the input text */
// 	long	bufsize;	/* size of the input text */
// 	long	curpos;		/* absolute char position in source */
// }	t_iterator;

/** 다음 문자가 있는지, eof는 아닌지 체크 */
bool has_next(const t_iterator *iter)
{
	char c;
	
	c = iter->buffer[iter->curpos];
	if (c != '\n' && c != '\0')
		return (true);
	else
		return (false);
}

/* move iterator to point next character */
char next(t_iterator *iter)
{
	char c1;

	c1 = '\0';
	if (!iter || !(iter->buffer))
	{
		errno = ENODATA;
		return (ERRCHAR);
	}
	if (iter->curpos == INIT_SRC_POS)
		iter->curpos = -1;
	else
		c1 = iter->buffer[iter->curpos];
	if (++iter->curpos >= iter->bufsize)
	{
		iter->curpos = iter->bufsize;
		return (EOF);
	}
	return (c1);
}

void unget(t_iterator *iter)
{
	if (iter->curpos < 0)
		return;
	iter->curpos--;
}

/** NOTE : cursur_pos를 움직이지 않고 다음 문자만 확인. */
char peek(t_iterator *iter)
{
	long	pos;

	if (!iter || !iter->buffer)
	{
		errno = ENODATA;
		return (ERRCHAR);
	}
	pos = iter->curpos;
	if (pos == INIT_SRC_POS)
		pos++;
	pos++;
	if (pos >= iter->bufsize)
		return (EOF);
	return (iter->buffer[pos]);
}

/* move iterator to point next character */
void	skip_white_space(t_iterator *iter)
{
	char	c;

	if (!iter || !iter->buffer)
		return ;
	/** 다음 문자가 EOF가 아니고 공백일 때 까지 */
	while (((c = peek(iter)) != EOF) && (ft_isspace(c)))
		next(iter);
}






