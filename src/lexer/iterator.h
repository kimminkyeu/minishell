/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:24:31 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 21:11:35 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_H
# define ITERATOR_H

# define EOF			(-1)
# define ERRCHAR		(0)

# define INIT_SRC_POS	(-2)

typedef struct s_iterator t_iterator;

typedef struct s_iterator {
	char	*buffer; 	/* the input text */
	long	bufsize;	/* size of the input text */
	long	curpos;		/* absolute char position in source */

}	t_iterator;

char	next(t_iterator *iter);

void	unget(t_iterator *iter);

char	peek(t_iterator *iter);

void	skip_white_space(t_iterator *iter);



#endif /* ITERATOR_H */
