/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 16:03:41 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/13 17:51:30 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	/** init list pointer and scanner */
	token_list = NULL;
	init_scanner(&scanner, line);

	/* while iterator meets '\n' or '\0', keep reading */
	while (scanner.f_has_next(&scanner))
	{
		/** (0) init new token to add to the list */
		token = new_token("");

		c = scanner.f_next(&scanner);

		/** (1) read one token ... */
		if (c == '|' && scanner.f_peek(&scanner) != '|')
			;// ... pipe |
		else if (c == '|' && scanner.f_peek(&scanner) == '|')
			;// ... ||
		else if (c == '&' && scanner.f_peek(&scanner) != '&')
			;// ... &
		else if (c == '&' && scanner.f_peek(&scanner) == '&')
			;// ... &&
		else if (c == '<' && scanner.f_peek(&scanner) != '<')
			;// ... <
		else if (c == '<' && scanner.f_peek(&scanner) == '<')
			;// ... <<
		else if (c == '>' && scanner.f_peek(&scanner) != '>')
			;// ... <
		else if (c == '>' && scanner.f_peek(&scanner) == '>')
			;// ... <<
		else if (c == '(' || c == ')')
			;// ... <
		else
			;// -> normal cmd or arg. use state machine

		/* NOTE : After this logic, string wiil be stored in token */

		/** (2) add token to list */
		ft_lstadd_back(&token_list, ft_lstnew(token));
	}
}
