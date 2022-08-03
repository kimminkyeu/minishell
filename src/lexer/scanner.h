/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:04:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/03 11:08:11 by yehan            ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_H
# define SCANNER_H

# include "../iterator/iterator.h"
# include "../lexer/token.h"

typedef struct s_scanner	t_scanner;

typedef struct s_scanner {
	t_iterator	iter;
	int			(*f_has_next)(t_scanner *scan);
	char		(*f_next)(t_scanner *scan);
	void		(*f_unget)(t_scanner *scan);
	char		(*f_peek)(t_scanner *scan);
	void		(*f_skip_white_space)(t_scanner *scan);
}	t_scanner;

t_list	*tokenize(char *line);

int		is_syntax_error(t_list *token_list);
void	init_scanner(t_scanner *scan, char *line);
void	set_redirection_type(t_list *token_list);
t_list	*create_initial_tokens(char *line);
bool	is_last_token_meta(t_list *token_list);
bool	is_bracket_unclosed(t_token *tok_1);
bool	is_meta_token_overlap(t_list *token_list);
bool	is_invalid_token_type(t_list *token_list);

/* Wrapper function of iterator. */
int		scanner_has_next(t_scanner *scan);
char	scanner_next(t_scanner *scan);
void	scanner_unget(t_scanner *scan);
char	scanner_peek(t_scanner *scan);
void	scanner_skip_white_space(t_scanner *scan);
bool	is_meta_token_type(t_token_type type);
bool	is_meta_char(char c);
void	get_pipe(t_token *tok, t_scanner *scan);
void	get_ampersand(t_token *tok, t_scanner *scan);
void	get_redirection(t_token *tok, t_scanner *scan);
void	get_double_quote(t_token *tok, t_scanner *scan);
void	get_single_quote(t_token *tok, t_scanner *scan);
void	get_semicolon(t_token *tok, t_scanner *scan);
void	get_bracket(t_token *tok, t_scanner *scan);
void	get_cmd_option(t_token *tok, t_scanner *scan);
void	get_cmd_or_arg(t_token *tok, t_scanner *scan);

#endif /* SCANNER_H */
