/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 12:26:31 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/14 14:22:30 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_H
# define STRING_H

# include "libft.h"

# define SUCCESS	(0)
# define ERROR		(1)
# define NO_ACTION	(2)

typedef struct s_string t_string;

typedef struct s_string {
	char		*text;
	size_t		text_len;
	size_t		capacity;

	int	(*f_append)(t_string *str, const char *str2);
	int (*f_push_back)(t_string *str, char c);
	int	(*f_pop_back)(t_string *str);
	int (*f_is_empty)(t_string *str);
	int	(*f_reserve)(t_string *str, size_t new_capacity);
	int	(*f_clear)(t_string *str);
	int	(*f_shrink_to_fit)(t_string *str);

}	t_string;

t_string	*new_string(size_t init_capacity);

void	delete_string(t_string **address_of_string);

/* @Add str2 to back. str1 + str2 
 * usage : str_assign(str_obj, get_next_line(STDIN));
 * --> 계속해서 입력값을 합침.
 * */
int	str_append(t_string *str, const char *str2);

/* n번째 index 앞에 str2 문자열을 삽입함. */
int	str_insert(t_string *str, int index, char *str2);

/* 저장된 문자열을 모두 지움 */
int	str_clear(t_string *str);

/* n~m index의 문자열을 지움 */
int	str_erase(t_string *str, int start_idx, int end_idx);

int	str_reserve(t_string *str, size_t new_capacity);

/* string의 capacity가 실제 사용하는 메모리보다 큰 경우 낭비되는 메모리가 없도록 메모리를 줄여줌 */
int	str_shrink_to_fit(t_string *str);

/*  str.reserve(n) */
/* size = n만큼의 메모리를 미리 할당해줌 */

int	str_is_empty(t_string *str);

/* str의 맨 뒤에 c 문자를 붙여줌 */
int	str_push_back(t_string *str, char c);

/* str의 맨 뒤의 문자를 제거 */
int	str_pop_back(t_string *str);

#endif
