/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 16:03:41 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/13 21:05:56 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "iterator.h"
#include "token.h"
#include "scanner.h"

/** TODO : move below functions to scanner_utils.c/h */
void	init_scanner(t_scanner *scan, char *line)
{
	init_iterator(&scan->iter, line);
	scan->state = E_PARSE_START;
	scan->f_has_next = scanner_has_next;
	scan->f_next = scanner_next;
	scan->f_unget = scanner_unget;
	scan->f_peek = scanner_peek;
	scan->f_skip_white_space = scanner_skip_white_space;
}

int		scanner_has_next(const t_scanner *scan)
{
	return (scan->iter.f_has_next(&scan->iter));
}

char	scanner_next(t_scanner *scan)
{
	return (scan->iter.f_next(&scan->iter));
}

void	scanner_unget(t_scanner *scan)
{
	scan->iter.f_unget(&scan->iter);
}

char	scanner_peek(t_scanner *scan)
{
	return (scan->iter.f_peek(&scan->iter));
}

void	scanner_skip_white_space(t_scanner *scan)
{
	scan->iter.f_skip_white_space(&scan->iter);
}
/** --------------------------------------------------------------- */

/** tokenizing functions */
t_token *get_pipe(t_scanner *scan)
{
	t_token	*tok;

	tok = new_token("");

	/** consume one letter (|) */
	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_PIPE;
	if (scan->f_peek(scan) == '|')
	{
		tok->push_back(tok, scan->f_next(scan));
		tok->type = E_TYPE_DOUBLE_PIPE;
	}
	return (tok);
}

t_token *get_ampersand(t_scanner *scan)
{
	t_token	*tok;

	tok = new_token("");




	return (tok);
}

t_token	*get_redirection(t_scanner *scan)
{
	t_token	*tok;

	return (tok);
}

t_token	*get_double_quote(t_scanner *scan)
{
	t_token	*tok;

	return (tok);
}

t_token	*get_single_quote(t_scanner *scan)
{
	t_token	*tok;

	return (tok);
}

t_token	*get_double_quote(t_scanner *scan)
{
	t_token	*tok;

	return (tok);
}

t_token	*get_bracket(t_scanner *scan)
{
	t_token	*tok;

	tok = new_token("|");
	return (tok);
}

t_token *get_simple_cmd(t_scanner *scan)
{
	t_token	*tok;

	tok = new_token("|");
	return (tok);
}









/** --------------------------------------------------------------- */

/* NOTE : scanner is the main logic of tokenizer
 * ----------------------------------------------
 *
 * (^ = space)
 *
 * 명령어 예시 1. [ ls^-al|grep^token>result<<heredoc ]
 * - 위 명령어는 heredoc의 입력과 ls-al의 입력 둘다 grep의 input pipe로 들어간다.
 *
 * 명령어 예시 2. [ echo 123 >result 456 ] 
 * - 위 명령어는 123456이 result로 들어간다.
 *
 * */


/** 입력된 문자열 토큰을 리스트로 반환. */
t_list	*tokenize(char *line)
{
	t_scanner	scanner;
	t_list		*token_list;
	t_token		*token;
	char		c;

	/** NOTE : 복잡하게 상태기계 쓰지 말고, 토큰 다 만들고 검사를 나중에 하자.  */
	/** init list pointer and scanner */
	token_list = NULL;
	init_scanner(&scanner, line);

	/* while iterator meets '\n' or '\0', keep reading */
	while (scanner.f_has_next(&scanner))
	{
		
		/** NOTE (1) : 규칙. cmd규칙과 관계 없이 무조건 기호 기준으로 자른다.  */

		/** NOTE (2) : rule of 우선순위 in brackets
		 * a='This string'
		 * ( a=banana; mkdir $a )
		 * echo $a
		 * # => 'This string'
		 * ls
		 * # => ...
		 * # => banana/ */

		/** (0) init new token to add to the list */
		
		token = new_token("");
		/** (1) read one token ... */
		c = scanner.f_peek(&scanner);
		if (c == '|')
			get_pipe(token, &scanner);
		else if (c == '&')
			get_ampersand(token, &scanner);
		else if (c == '<' || c == '>')
			get_redirection(token, &scanner);
		else if (c == '\"')
			get_double_quote(token, &scanner); // NOTE : if scanner doesn't find pair-quote, then error.
		else if (c == '\'')
			get_single_quote(token, &scanner); // NOTE : 다른 ' 를 만나기 전까지 그 전체를 감싸준다.
		else if (c == '(' || c == ')') // 예는 감싸지 말고, 그냥 bracket처리만 하자.
			get_bracket(token, &scanner); // NOTE : 만약 bracket이 규칙이 안맞는다면 에러 처리 (...) ((...)) (((..)))
		else
			get_simple_cmd(token, &scanner); // move scanner iterator, them return token



		/** if (c == '|' && scanner.f_peek(&scanner) != '|')
		  *     token = get_pipe(&scanner);
		  * else if (c == '|' && scanner.f_peek(&scanner) == '|')
		  *     token = get_double_pipe(&scanner);
		  * else if (c == '&' && scanner.f_peek(&scanner) != '&')
		  *     token = get_single_ampersand(&scanner);
		  * else if (c == '&' && scanner.f_peek(&scanner) == '&')
		  *     token = get_double_ampersand(&scanner);
		  * else if (c == '<' && scanner.f_peek(&scanner) != '<')
		  *     token = get_redirection_in(&scanner);
		  * else if (c == '<' && scanner.f_peek(&scanner) == '<')
		  *     token = get_redirection_heredoc(&scanner);
		  * else if (c == '>' && scanner.f_peek(&scanner) != '>')
		  *     token = get_redirection_out_overwrite(&scanner);
		  * else if (c == '>' && scanner.f_peek(&scanner) == '>')
		  *     token = get_redirection_out_append(&scanner);
		  * else if (c == '\"')
		  *     token = get_double_quote_string(&scanner); // NOTE : if scanner doesn't find pair-quote, then error.
		  * else if (c == '\'')
		  *     token = get_single_quote_string(&scanner); // NOTE : 다른 ' 를 만나기 전까지 그 전체를 감싸준다.
		  * else if (c == '(' || c == ')') // 예는 감싸지 말고, 그냥 bracket처리만 하자.
		  *     token = get_bracket(&scanner); // NOTE : 만약 bracket이 규칙이 안맞는다면 에러 처리 (...) ((...)) (((..)))
		  * else
		  *     token = get_simple_cmd(&scanner); // move scanner iterator, them return token */


		/** if one of the token type is error, then stop. */
		/** if (scanner.state == E_PARSE_ERROR)
		  * {
		  *     printf("Syntax Error.\t[TODO] : change this to shell-like messege\n");
		  *     return (NULL);
		  * } --> FIXME : 토큰화 단계에선 오류 체크를 하지 말것. 다 하고 나서 하자.*/

		/** NOTE : After this logic, string wiil be stored in token */
		/** (2) add token to list */
		ft_lstadd_back(&token_list, ft_lstnew(token));

		/** NOTE : Stored token list would be [ls -al][|][echo] */
	}
}
