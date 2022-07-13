/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 16:03:41 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 00:17:34 by minkyeki         ###   ########.fr       */
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

int		scanner_has_next(t_scanner *scan)
{
	return (scan->iter.f_has_next(&(scan->iter)));
}

char	scanner_next(t_scanner *scan)
{
	/** char tmp = scan->iter.f_next(&(scan->iter)); */
	/** printf("scanner_next return val : %c\n", tmp); */
	return (scan->iter.f_next(&(scan->iter)));
	/** return (tmp); */
}

void	scanner_unget(t_scanner *scan)
{
	scan->iter.f_unget(&(scan->iter));
}

char	scanner_peek(t_scanner *scan)
{
	return (scan->iter.f_peek(&(scan->iter)));
}

void	scanner_skip_white_space(t_scanner *scan)
{
	scan->iter.f_skip_white_space(&(scan->iter));
}
/** --------------------------------------------------------------- */

/** tokenizing functions */
void	get_pipe(t_token *tok, t_scanner *scan)
{
	/** consume one letter (|) */
	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_PIPE;
	if (scan->f_peek(scan) == '|')
	{
		tok->push_back(tok, scan->f_next(scan));
		tok->type = E_TYPE_DOUBLE_PIPE;
	}
}

void	get_ampersand(t_token *tok, t_scanner *scan)
{
	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_AMPERSAND;
	if (scan->f_peek(scan) == '&')
	{
		tok->push_back(tok, scan->f_next(scan));
		tok->type = E_TYPE_DOUBLE_AMPERSAND;
	}
}

void	get_redirection(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_REDIRECT;

	/** TODO : Error checking later!! */
	c = scan->f_peek(scan);
	if (c == '<' || c == '>')
	{
		tok->push_back(tok, scan->f_next(scan));
		tok->type = E_TYPE_DOUBLE_AMPERSAND;
	}
}

void	get_double_quote(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_DOUBLE_QUOTE;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (c == '\"')
		{
			tok->push_back(tok, scan->f_next(scan));
			break ;
		}
		else
		{
			tok->push_back(tok, scan->f_next(scan));
		}
	}
}

void	get_single_quote(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_SINGLE_QUOTE;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (c == '\'')
		{
			tok->push_back(tok, scan->f_next(scan));
			break ;
		}
		else
		{
			tok->push_back(tok, scan->f_next(scan));
		}
	}
}

void	get_semicolon(t_token *tok, t_scanner *scan)
{
	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_SEMICOLON;
}

void	get_bracket(t_token *tok, t_scanner *scan)
{
	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_BRACKET;
}

void	get_cmd_option(t_token *tok, t_scanner *scan)
{
	char	c;

	/**  */
	/** printf("option found\n"); */
	tok->push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_CMD_OPTION;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (!ft_isspace(c) \
				&& c != '|' && c != '&' && c != '<' && c != '>' \
				&& c != '(' && c != ')' && c != ';' \
				&& c != '\'' && c != '\"')
		{
			tok->push_back(tok, scan->f_next(scan));
		}
		else
			break ;
	}

}

void	get_cmd_or_arg(t_token *tok, t_scanner *scan)
{
	char	c;

	char	tmp = scan->f_next(scan);
	tok->push_back(tok, tmp);
	/** printf("first f_next : (%c)\n", tmp); */

	/** tok->push_back(tok, scan->f_next(scan)); */
	/** printf("%s ->", tok->str->text); */
	tok->type = E_TYPE_CMD_OR_ARG;
	/** printf("inside cmd_func f_peek val : (%c)\n", c); */
	while (scan->f_has_next(scan))
	{
		/** printf("inside cmd_func f_next val : (%c)\n", tmp); */
		// echo     -n    hello     | ...
		c = scan->f_peek(scan);
		if (!ft_isspace(c) \
				&& c != '|' && c != '&' && c != '<' && c != '>' \
				&& c != '(' && c != ')' && c != ';' \
				&& c != '\'' && c != '\"')
		{
			tok->push_back(tok, scan->f_next(scan));
			/** printf("%s ->", tok->str->text); */
		}
		else
			break ;
	}
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

/** NOTE (1) : 규칙. cmd규칙과 관계 없이 무조건 기호 기준으로 자른다.  */
		/** NOTE (2) : rule of 우선순위 in brackets
		 * a='This string'
		 * ( a=banana; mkdir $a )
		 * echo $a
		 * # => 'This string'
		 * ls
		 * # => ...
		 * # => banana/ */



/** 입력된 문자열 토큰을 리스트로 반환. */
/** NOTE : 복잡하게 상태기계 쓰지 말고, 토큰 다 만들고 검사를 나중에 하자.  */
t_list	*tokenize(char *line)
{
	t_scanner	scanner;
	t_list		*token_list;
	t_token		*token;
	char		c;

	/** init list pointer and scanner */
	token_list = NULL;
	init_scanner(&scanner, line);
	while (scanner.f_has_next(&scanner))
	{
		/** (0) init new token to add to the list */
		if (ft_isspace(scanner.f_peek(&scanner)))
		{
			scanner.f_skip_white_space(&scanner);
			continue ;
		}

		c = scanner.f_peek(&scanner);
		/** printf("c: %c (ascii=%d)\n", c, c); */
		token = new_token("");
		/** (1) read one token ... */

		if (c == '|')
			get_pipe(token, &scanner);
		else if (c == '&')
			get_ampersand(token, &scanner);
		else if (c == ';')
			get_semicolon(token, &scanner);
		else if (c == '<' || c == '>')
			get_redirection(token, &scanner);
		else if (c == '\"')
			get_double_quote(token, &scanner); // NOTE : if scanner doesn't find pair-quote, then error.
		else if (c == '\'')
			get_single_quote(token, &scanner); // NOTE : 다른 ' 를 만나기 전까지 그 전체를 감싸준다.
		else if (c == '(' || c == ')')
			get_bracket(token, &scanner);
		else if (c == '-')
			get_cmd_option(token, &scanner);
		else
			get_cmd_or_arg(token, &scanner); // move scanner iterator, them return token
		/** (2) add token to list */
		ft_lstadd_back(&token_list, ft_lstnew(token));
	}
	return (token_list);
}

// TODO : Test case to check.
// "ls -al | grep hello     && echo -n 'hi' >out <<DELIM "
