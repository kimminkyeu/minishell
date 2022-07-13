/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:04:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/13 17:07:30 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

typedef enum e_state {
	E_PARSE_START,		// default state
	E_PARSE_OPTION,		// ls => -al
	E_PARSE_ARG,		// double or signle quote + CMD_Argument
	E_PARSE_CMD,
	E_PARSE_META_CHAR,
	// E_PARSE_WHITESPACE,
	E_PARSE_ERROR,
	// ... Add accordingly
}	t_state;

typedef struct s_scanner	t_scanner;

typedef struct s_scanner {
	t_state		state;
	t_iterator	iter;

	int		(*f_has_next)(const t_scanner *scan);
	char	(*f_next)(t_scanner *scan);
	void	(*f_unget)(t_scanner *scan);
	char	(*f_peek)(t_scanner *scan);
	void	(*f_skip_white_space)(t_scanner *scan);
}	t_scanner;

void	init_scanner(t_scanner *scan, char *line);

/* Wrapper function of iterator. */
int		scanner_has_next(const t_scanner *scan);
char	scanner_next(t_scanner *scan);
void	scanner_unget(t_scanner *scan);
char	scanner_peek(t_scanner *scan);
void	scanner_skip_white_space(t_scanner *scan);

#endif /* SCANNER_H */
