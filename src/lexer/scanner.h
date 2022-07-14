/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:04:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 22:18:56 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

# include "iterator.h"

typedef struct s_scanner	t_scanner;

typedef struct s_scanner {
	t_iterator	iter;

	int		(*f_has_next)(t_scanner *scan);
	char	(*f_next)(t_scanner *scan);
	void	(*f_unget)(t_scanner *scan);
	char	(*f_peek)(t_scanner *scan);
	void	(*f_skip_white_space)(t_scanner *scan);
}	t_scanner;


t_list	*tokenize(char *line);

void	init_scanner(t_scanner *scan, char *line);

/* Wrapper function of iterator. */
int		scanner_has_next(t_scanner *scan);
char	scanner_next(t_scanner *scan);
void	scanner_unget(t_scanner *scan);
char	scanner_peek(t_scanner *scan);
void	scanner_skip_white_space(t_scanner *scan);


void	get_pipe(t_token *tok, t_scanner *scan);
void	get_double_ampersand(t_token *tok, t_scanner *scan);
void	get_redirection(t_token *tok, t_scanner *scan);
void	get_double_quote(t_token *tok, t_scanner *scan);
void	get_single_quote(t_token *tok, t_scanner *scan);
void	get_semicolon(t_token *tok, t_scanner *scan);
void	get_bracket(t_token *tok, t_scanner *scan);
void	get_cmd_option(t_token *tok, t_scanner *scan);
void	get_cmd_or_arg(t_token *tok, t_scanner *scan);

#endif /* SCANNER_H */
