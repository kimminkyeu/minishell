/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 16:03:41 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/17 16:56:59 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../../include/libft.h"
#include "../iterator/iterator.h"
#include "token.h"
#include "scanner.h"

/** TODO : move below functions to scanner_utils.c/h */
void	init_scanner(t_scanner *scan, char *line)
{
	init_iterator(&scan->iter, line);
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
	return (scan->iter.f_next(&(scan->iter)));
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

bool	is_meta_char(char c)
{
	if (c != '|' && c != '&' && c != '<' && c != '>' \
			&& /*c != '(' && c != ')' &&*/ c != ';' \
			/* && c != '\'' && c != '\"' */ )
		return (false);
	else
		return (true);
}

void	get_pipe(t_token *tok, t_scanner *scan)
{
	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_PIPE;
	if (scan->f_peek(scan) == '|')
	{
		tok->f_push_back(tok, scan->f_next(scan));
		tok->type = E_TYPE_DOUBLE_PIPE;
	}
}

void	get_double_ampersand(t_token *tok, t_scanner *scan)
{
	/** add to token, only if - double ampersand */
	char	c1;
	char	c2;

	c1 = scan->f_next(scan);
	c2 = scan->f_next(scan);
	if (c1 == '&' && c2 == '&')
	{
		tok->f_push_back(tok, c1);
		tok->f_push_back(tok, c2);
		tok->type = E_TYPE_DOUBLE_AMPERSAND;
	}
	else
	{
		scan->f_unget(scan);
		scan->f_unget(scan);
		get_cmd_or_arg(tok, scan);
	}
}

void	get_redirection(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	tok->type = E_TYPE_REDIRECT;
	c = scan->f_peek(scan);
	if (c == '<' || c == '>')
		tok->f_push_back(tok, scan->f_next(scan));
}

void	get_double_quote(t_token *tok, t_scanner *scan)
{
	char	c;

	// tok->type = E_TYPE_DOU_QUOTE;
	tok->f_push_back(tok, scan->f_next(scan));
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (c == '\"')
			break ;
		tok->f_push_back(tok, scan->f_next(scan));
	}
	tok->f_push_back(tok, scan->f_next(scan));
}

void	get_single_quote(t_token *tok, t_scanner *scan)
{
	char	c;

	// tok->type = E_TYPE_SINGLE_QUOTE;
	tok->f_push_back(tok, scan->f_next(scan));
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (c == '\'')
			break ;
		tok->f_push_back(tok, scan->f_next(scan));
	}
	tok->f_push_back(tok, scan->f_next(scan));
}

void	get_dollar_sign(t_token *tok, t_scanner *scan)
{
	char	c;

	tok->f_push_back(tok, scan->f_next(scan));
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);
		if (ft_isspace(c) || is_meta_char(c) || c == '\"' || c == '\'')
			break ;
		tok->f_push_back(tok, scan->f_next(scan));
	}
}

/** void	get_semicolon(t_token *tok, t_scanner *scan)
  * {
  *     tok->f_push_back(tok, scan->f_next(scan));
  *     tok->type = E_TYPE_SEMICOLON;
  * } */

void	get_bracket(t_token *tok, t_scanner *scan)
{
	int	count;
	int	c;

	count = 0;
	tok->type = E_TYPE_BRACKET;
	while (scan->f_has_next(scan))
	{
		c = scan->f_next(scan);
		tok->f_push_back(tok, c);
		if (c == '(')
			count++;
		else if (c == ')')
			count--;
		if (count == 0)
			break ;
	}
}

/** void	get_cmd_option(t_token *tok, t_scanner *scan)
  * {
  *     char	c;
  *
  *     tok->f_push_back(tok, scan->f_next(scan));
  *     tok->type = E_TYPE_CMD_OPTION;
  *     while (scan->f_has_next(scan))
  *     {
  *         c = scan->f_peek(scan);
  *         if (!ft_isspace(c) && !is_meta_char(c) && c != '\'' && c != '\"' \
  *                 && c != '(' && c != ')')
  *             tok->f_push_back(tok, scan->f_next(scan));
  *         else
  *             break ;
  *     }
  *
  * } */


void	get_cmd_or_arg(t_token *tok, t_scanner *scan)
{
	char	c; 

	tok->type = E_TYPE_SIMPLE_CMD;
	while (scan->f_has_next(scan))
	{
		c = scan->f_peek(scan);

		if (c == '\'')
			get_single_quote(tok, scan);
		else if (c == '\"')
			get_double_quote(tok, scan);
		else if (c == '$')
			get_dollar_sign(tok, scan);
		else if (ft_isspace(c) == false && is_meta_char(c) == false)
			tok->f_push_back(tok, scan->f_next(scan));
		else
			break ;
	}
}

t_list	*create_initial_tokens(char *line)
{
	t_scanner	scanner;
	t_list		*token_list;
	t_token		*token;
	char		c;

	token_list = NULL;
	init_scanner(&scanner, line);
	while (scanner.f_has_next(&scanner))
	{
		if (ft_isspace(scanner.f_peek(&scanner)))
		{
			scanner.f_skip_white_space(&scanner);
			continue ;
		}
		c = scanner.f_peek(&scanner);
		token = new_token("");
		if (c == '|')
			get_pipe(token, &scanner);
		else if (c == '&')
			get_double_ampersand(token, &scanner);
		else if (c == '<' || c == '>')
			get_redirection(token, &scanner);
		// else if (c == '\"')
		// 	get_double_quote(token, &scanner);
		// else if (c == '\'')
		// 	get_single_quote(token, &scanner);
		else if (c == '(')
			get_bracket(token, &scanner);
		/** else if (ft_isspace(c)) */
			/** get_whitespace(token, &scanner); */
		else
			get_cmd_or_arg(token, &scanner);
		ft_lstadd_back(&token_list, ft_lstnew(token));
	}
	return (token_list);
}

	/** 완성된 리스트를 돌면서 1차 검토, redir 옆은 redir_arg로 처리함. */
void	set_redirection_arg(t_list *token_list)
{
	t_list	*tmp;
	t_token	*tok;

	tmp = token_list;
	while (tmp->next != NULL)
	{
		tok = tmp->content;
		if (tok->type == E_TYPE_REDIRECT)
		{
			tmp = tmp->next;
			tok = tmp->content;
			tok->type = E_TYPE_REDIR_ARG;
			continue ;
		}
		tmp = tmp->next;
	}
}

bool	is_meta_token_type(t_token_type type)
{
	if (type != E_TYPE_PIPE && type != E_TYPE_DOUBLE_PIPE && \
			type != E_TYPE_REDIRECT && type != E_TYPE_DOUBLE_AMPERSAND)
		return (false);
	else
		return (true);
}

/** TODO : 아래 신택스 체킹 진행하기 */
int	is_syntax_error(t_list *token_list)
{
	/** TODO :신택스 에러 체킹 */
/**   done  (1) >이후 토큰이 끝나고 파일이 없을 경우 bash와 같은 신택스 에러 출력
  *   done (2) echo ||| 이렇게 될 경우 |문자에 대한 신택스 에러 출력.
  *         즉 파이프 다음에 파이프가 또 나올 경우,
  *         즉 메타 캐릭터(&& || |)토큰 이후 바로 또 다른 메타캐릭터 토큰이 나온 경우
  *    done (3) 괄호가 안닫힌 경우 (a
  *     (3) ( ) 괄호 안에 괄호가 하나라도 있을 경우 신택스 에러 출력
  *     (4) (a)(b)는 )에 대한 신택스 에러 출력
  *     (5) (a) | (b) 는 신택스 에러 없음. 실행부로 넘어감.
  *     (6) (a)는 신택스 에러 없음. 실행부로 넘어감.
  *  */

	t_list	*cur;
	t_token	*tok_1;
	t_token	*tok_2;
	char	c;

	cur = token_list;

	while (cur->next != NULL)
	{
		tok_1 = cur->content;
		tok_2 = cur->next->content;
		c = tok_2->str->text[0];

		/** 만약 && || | > 이 서로 연속된다면 */
		if (is_meta_token_type(tok_1->type) && is_meta_token_type(tok_2->type))
		{
			printf("lesh: syntax error near unexpected token '%c'\n", c);
			return (true);
		}

		cur = cur->next;
	}

	/** 여기를 빠져나온 노드는 마지막 노드 */
	tok_1 = cur->content;
	if (tok_1->type == E_TYPE_REDIRECT)
	{
		printf("lesh: syntax error near unexpected token `newline'\n");
		return (true);
	}
	else if (tok_1->type == E_TYPE_BRACKET)
	{
		c = tok_1->str->text[tok_1->str->text_len - 1];
		if (c != ')') // 만약 괄호가 안닫힌 경우
		{
			printf("lesh: syntax error near unexpected token `%c'\n", c);
			return (true);
		}
		else // 괄호 안에 괄호가 또 있는지 체크
		{
			char	*tmp;

			/** FIXME : 여기서 세그폴드가 뜰수 도 있다. */
			tmp = &(tok_1->str->text[1]); // 두번째 문자부터 확인 시작
			while (*tmp != '\0')
			{
				if (*tmp == '(')
				{
					tmp++;
					printf("lesh: syntax error near unexpected token '");
					while (*tmp != ')' && *tmp != '\0')
					{
						printf("%c", *tmp);
						tmp++;
					}
					printf("'\n");
					return (true);
				}
				tmp++;
			}
		}
	}
	return (false);
}

t_list	*tokenize(char *line)
{
	t_list		*token_list;

	/** (0) 초기 토큰 리스트 생성. */
	token_list = create_initial_tokens(line);

	/** (2) 에러 신택스 검사. 만약 에러가 발생하면 token 전부 free하고 null 반환
	 * NOTE : 오류 메시지는 is_syntax_error 함수 내부에서 출력해줌.
	 * */
	if (is_syntax_error(token_list))
	{
		ft_lstclear(&token_list, delete_token);
		return (NULL);
	}

	/** (1) redirection argument 세팅. --> 트리 만들때 필요 */
	set_redirection_arg(token_list);

	return (token_list);
}
