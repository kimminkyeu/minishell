/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:39:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/28 01:51:09 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "../../include/libft.h"
# include "../string/string.h"
# include "../iterator/iterator.h"

typedef enum e_token_type {
	E_TYPE_DOUBLE_AMPERSAND,
	E_TYPE_DOUBLE_PIPE,
	E_TYPE_PIPE,
	E_TYPE_BRACKET,
	E_TYPE_SIMPLE_CMD,
	E_TYPE_REDIR_GREATER,
	E_TYPE_REDIR_LESS,
	E_TYPE_REDIR_APPEND,
	E_TYPE_REDIR_HEREDOC,
	E_TYPE_REDIR_ARG,
	E_TYPE_WHITESPACE,
	E_TYPE_DEFAULT,
	E_TYPE_REDIRECT,
	/* NOTE: 타입 추가는 밑에 해주세요. (위 순서가 바뀌면 안됨) */
	/* E_TYPE_... */

}	t_token_type;

typedef struct s_token	t_token;

typedef struct s_token {
	t_token_type	type;
	t_string		*str;
	void			(*f_reset)(t_token *token);
	void			(*f_push_back)(t_token *token, char c);
	void			(*f_pop_back)(t_token *token);
	void			(*f_append)(t_token *token, const char *str_in);
}	t_token;

t_token	*new_token(char *str_input);
void	delete_token(void *token);

void	token_reset(t_token *token);
void	token_push_back(t_token *token, char c);
void	token_pop_back(t_token *token);
void	token_append(t_token *token, const char *str_in);

#endif /* TOKEN_H */
