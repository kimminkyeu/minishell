/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:24:31 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/22 23:14:23 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_H
# define ITERATOR_H

# define EOF			(-1)
# define ERRCHAR		(0)

# define INIT_SRC_POS	(-2)

# include <errno.h>
# include "../../include/libft.h"

typedef struct s_iterator t_iterator;

typedef struct s_iterator {
	char	*line;		/* the input text */
	long	line_len;	/* size of the input text */
	long	curpos;		/* iterator position is line */

	int		(*f_has_next)(t_iterator *iter);
	char	(*f_next)(t_iterator *iter);
	void	(*f_unget)(t_iterator *iter);
	char	(*f_peek)(t_iterator *iter);
	void	(*f_skip_white_space)(t_iterator *iter);
}	t_iterator;

void	init_iterator(t_iterator *iter, char *line);

int		iter_has_next(t_iterator *iter);
char	iter_next(t_iterator *iter);
void	iter_unget(t_iterator *iter);
char	iter_peek(t_iterator *iter);
void	iter_skip_white_space(t_iterator *iter);

#endif /* ITERATOR_H */
