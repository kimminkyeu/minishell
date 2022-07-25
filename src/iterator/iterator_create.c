/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:48:17 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 17:48:52 by minkyeki         ###   ########.fr       */
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
