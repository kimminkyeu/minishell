/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 18:24:00 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/10 19:52:58 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_H
# define STRING_H

# include "../include/libft.h"

typedef struct s_string {
	size_t	size;
	size_t	capacity;
	char	*string;

	/* Function Pointer */
	void	(*append)(t_string *str, char *str2);
	void	(*insert)(t_string *str, int index, char *str2);
	void	(*clear)(t_string *str);
	void	(*erase)(t_string *str, int start_idx, int end_idx);
	void	(*push_back)(t_string *str, char c);
	void	(*pop_back)(t_string *str);
}	t_string;

int			is_empty(t_string *str)
{
	if (str->size == 0)
		return (1);
	else
		return (0);
}

t_string	*new_string(size_t init_capacity)
{
	t_string	*new_str;

	*new_str = ft_calloc(1, sizeof(*new_str));
	if (*new_str)
	{
		(new_str)->size = 0;
		(new_str)->capacity = init_capacity;
		(new_str)->string = ft_calloc((*new_array)->capacity, sizeof(char));
		if (!(new_str->string))
		{
			free((*new_array)->data);
			free(*new_array);
			*new_array = NULL;
		}
	}
	return (*new_array);
}

/* @Add str2 to back. str1 + str2 
 * usage : str_assign(str_obj, get_next_line(STDIN));
 * --> 계속해서 입력값을 합침.
 * */
void	str_append(t_string *str, char *str2);

/* n번째 index 앞에 str2 문자열을 삽입함. */
void	str_insert(t_string *str, int index, char *str2);

/* 저장된 문자열을 모두 지움 */
void	str_clear(t_string *str);

/* n~m index의 문자열을 지움 */
void	str_erase(t_string *str, int start_idx, int end_idx);

/* str의 맨 뒤에 c 문자를 붙여줌 */
void	str_push_back(t_string *str, char c)

/* str의 맨 뒤의 문자를 제거 */
void	str_pop_back(t_string *str)

/* string의 capacity가 실제 사용하는 메모리보다 큰 경우 낭비되는 메모리가 없도록 메모리를 줄여줌 */
void	str_shrink_to_fit(t_string *str);

/*  str.reserve(n) */
/* size = n만큼의 메모리를 미리 할당해줌 */

#endif /* STRING_H */
