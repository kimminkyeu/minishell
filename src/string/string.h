/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 12:26:31 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/25 17:38:51 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_H
# define STRING_H

# include "../../include/libft.h"

# define SUCCESS	(0)
# define ERROR		(1)
# define NO_ACTION	(2)

typedef struct s_string t_string;

/* ---------------------------
 * @ Type define for t_string
 * . 
 * - *text     : pointer to string.
 * - text_len  : number of chararaters filled in *text.
 * - capacity  : total size of it's allocated memory. */
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
	int	(*f_replace)(t_string *str, size_t pos, size_t len, const char *str_to_replace);
	int	(*f_replace_all)(t_string *str, const char *substr_old, const char *substr_new);
}	t_string;

/* ============================
 * @ Default Constructor.
 * NOTE: do not use malloc. 
 * NOTE: [ t_vector *vec = new_vector( "#capacity you want" ) ];
 *       --> returns NULL on error. */
t_string	*new_string(size_t init_capacity);

/* ============================
 * @Default Destructor.
 * NOTE: Deletes(free) everything, including pointer passed as argument. 
 * NOTE: Use this function instead of calling free() yourself !! */
void	delete_string(t_string **address_of_string);

/* @Add str2 to back. Result = str + str2 */
int	str_append(t_string *str, const char *str2);

/* 저장된 문자열을 모두 지움 */
int	str_clear(t_string *str);

/* size = n만큼의 메모리를 미리 할당해줌 */
int	str_reserve(t_string *str, size_t new_capacity);

/* string의 capacity가 실제 사용하는 메모리보다 큰 경우 낭비되는 메모리가 없도록 메모리를 줄여줌 */
int	str_shrink_to_fit(t_string *str);

/* If string is empty, function returns (true) */
int	str_is_empty(t_string *str);

/* str의 맨 뒤에 c 문자를 붙여줌 */
int	str_push_back(t_string *str, char c);

/* str의 맨 뒤의 문자를 제거 */
int	str_pop_back(t_string *str);

/** Replaces the portion of the string that begins at character pos and spans len characters 
 * (or the part of the string in the range between [i1,i2)) by new contents: */
int	str_replace(t_string *str, size_t pos, size_t len, const char *str_to_replace);

/** Edit string with all matches of a pattern replaced by a replacement 
 *  (ex. INPUT ["hi kyeu. hi again!"] 
 *  --> str_replace_all(str, "hi", "hello")
 *  --> OUTPUT["hello kyeu. hello again!")]
 * */
int	str_replace_all(t_string *str, const char *substr_old, const char *substr_new);

#endif /* STRING_H */
