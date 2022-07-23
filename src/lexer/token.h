/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:39:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/23 20:58:55 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "../../include/libft.h"
# include "../string/string.h"
# include "../iterator/iterator.h"

typedef enum e_token_type {
								//
	E_TYPE_DOUBLE_AMPERSAND,	//	[&&]
	E_TYPE_DOUBLE_PIPE, 		//  [||]
	E_TYPE_PIPE,				//	[|]
	E_TYPE_BRACKET,				//  [(] [)] --> for subshell
	E_TYPE_SIMPLE_CMD,			// simple_command
								//
	E_TYPE_REDIR_GREATER,		//	[>]
	E_TYPE_REDIR_LESS,			//	[<]
	E_TYPE_REDIR_APPEND,		//	[>>]
	E_TYPE_REDIR_HEREDOC,		//	[<<]
	E_TYPE_REDIR_ARG,			//	[infile outfile]
								//
	E_TYPE_WHITESPACE, 			// 공백 토큰
	E_TYPE_DEFAULT, 			// No type
	E_TYPE_REDIRECT
}	t_token_type;

typedef struct s_token	t_token;

typedef struct s_token {
	// t_iterator		*src; /* --> NOTE : Really Needed? */
	t_token_type	type;
	t_string		*str;

	void	(*f_reset)(t_token *token);						/* 모든 토큰 초기화 */
	void	(*f_push_back)(t_token *token, char c);			/* 토큰에 문자 추가 */
	void	(*f_pop_back)(t_token *token);					/* 토큰의 마지막 문자 제거 */
	void	(*f_append)(t_token *token, const char *str_in);	/* 토큰에 문자열 추가 */
}	t_token;

t_token	*new_token(char *str_input);
void	delete_token(void *token);

void	token_reset(t_token *token);
void	token_push_back(t_token *token, char c);
void	token_pop_back(t_token *token);
void	token_append(t_token *token, const char *str_in);

#endif /* TOKEN_H */
