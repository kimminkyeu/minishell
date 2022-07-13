/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:39:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/13 17:31:05 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "libft.h"
# include "string.h"
# include "iterator.h"

typedef enum e_token_type {
	E_TYPE_DEFAULT, 			// No type
	E_TYPE_SPACE, 			// isspace()
	E_TYPE_CMD,				// simple_command
	E_TYPE_ARG,				// argument -> ex) echo [hello] --> hello = argument

	E_TYPE_SINGLE_QUOTE, 	//  [']
	E_TYPE_DOUBLE_QUOTE,		//  ["]

	E_TYPE_PIPE,				//	[|]
	E_TYPE_REDIRECT,			//	[< << > >>]

	E_TYPE_BRACKET,			//	[(] [)]
	E_TYPE_SEMICOLON,		//	[;]
	E_TYPE_DOUBLE_AMPERSAND, //	[&&]
	E_TYPE_DOUBLE_PIPE, 		//  [||]
	E_TYPE_VAR,				//	[$VAR]
					
	E_TYPE_EOF				//  [ EOF special token like NULL ]
}	t_token_type;

typedef struct s_token	t_token;

typedef struct s_token {
	// t_iterator		*src; /* --> NOTE : Really Needed? */
	t_token_type	type;
	t_string		*str;

	void	(*reset)(t_token *token);						/* 모든 토큰 초기화 */
	void	(*push_back)(t_token *token, char c);			/* 토큰에 문자 추가 */
	void	(*pop_back)(t_token *token);					/* 토큰의 마지막 문자 제거 */
	void	(*append)(t_token *token, const char *str_in);	/* 토큰에 문자열 추가 */
}	t_token;

t_token	*new_token(char *str_input);
void	delete_token(void *token);

void	token_reset(t_token *token);
void	token_push_back(t_token *token, char c);
void	token_pop_back(t_token *token);
void	token_append(t_token *token, const char *str_in);

#endif /* TOKEN_H */
