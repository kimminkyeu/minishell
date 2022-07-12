/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:24:31 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 13:28:49 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_H
# define ITERATOR_H

typedef struct s_iterator {
	char	*buffer; 	/* the input text */
	long	bufsize;	/* size of the input text */
	long	curpos;		/* absolute char position in source */

	char	(*next)(t_iterator *iter);
	void	(*prev)(t_iterator *iter);
	char	(*peek)(t_iterator *iter);
	void	(*skip_white_space)(t_iterator *iter);

}	t_iterator;

char	next(t_iterator *iter);
void	prev(t_iterator *iter);
char	peek(t_iterator *iter);
void	skip_white_space(t_iterator *iter);



#endif /* ITERATOR_H */