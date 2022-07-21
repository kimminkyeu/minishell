/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 12:30:54 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/21 17:50:11 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "string.h"

t_string	*new_string(size_t init_capacity)
{
	t_string	*str;

	if (init_capacity <= 0)
		return (NULL);
	str = ft_calloc(1, sizeof(*str));
	if (str == NULL)
		return (NULL);
	str->text = ft_calloc(init_capacity, sizeof(*(str->text)));
	if (str->text == NULL)
		return (NULL);
	str->text_len = 0;
	str->capacity = init_capacity;

	str->f_append = str_append;
	str->f_clear = str_clear;
	str->f_is_empty = str_is_empty;
	str->f_pop_back = str_pop_back;
	str->f_push_back = str_push_back;
	str->f_reserve = str_reserve;
	str->f_shrink_to_fit = str_shrink_to_fit;
	str->f_replace = str_replace;
	str->f_replace_all = str_replace_all;

	return (str);
}

void	delete_string(t_string **address_of_string)
{
	if (*address_of_string != NULL)
		free((*address_of_string)->text);
	free(*address_of_string);
	address_of_string = NULL;
}

int	str_clear(t_string *str)
{
	if (str_is_empty(str))
		return (NO_ACTION);
	ft_bzero(str->text, str->capacity);
	str->text_len = 0;
	return (SUCCESS);
}

int	str_shrink_to_fit(t_string *str)
{
	if (str->text_len >= (str->capacity - 1))
		return (NO_ACTION);
	if (str_reserve(str, str->text_len + 1) != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}


int	str_push_back(t_string *str, char c)
{
	/** NOTE : Always make last element null */
	if (str->text_len >= str->capacity - 1)
	{
		if (str_reserve(str, str->capacity * 2) != SUCCESS)
			return (NO_ACTION);
	}
	str->text[str->text_len] = c;
	str->text_len++;
	return (SUCCESS);
}

int		str_pop_back(t_string *str)
{
	if (str->text_len > 0)
	{
		/** NOTE : Always make last element null */
		str->text[str->text_len - 1] = '\0';
		str->text_len--;
		return (SUCCESS);
	}
	return (NO_ACTION);
}

int	str_is_empty(t_string *str)
{
	if (str->text_len == 0)
		return (true);
	else
		return (false);
}

int str_reserve(t_string *str, size_t new_capacity)
{
	char	*new_text;

	/* a b c 0 0 [len = 3] --> a b c 0 [new_capa = 4] */
	if (str->text_len > new_capacity - 1)
		return (NO_ACTION);
	new_text = ft_calloc(new_capacity, sizeof(*new_text));
	if (new_text == NULL)
		return (ERROR);
	ft_memmove(new_text, str->text, str->text_len);
	free(str->text);
	str->text = new_text;
	str->capacity = new_capacity;
	return (SUCCESS);
}

int	str_append(t_string *str, const char *str2)
{
	size_t	str2_len;

	if (str2 == NULL)
		return (NO_ACTION);
	str2_len = ft_strlen(str2);
	if (str2_len == 0)
		return (NO_ACTION);
	if (str->capacity - 1 < (str->text_len + str2_len))
	{
		if (str_reserve(str, (str->capacity * 2) + str2_len) != SUCCESS)
			return (ERROR);
	}
	ft_memmove(&(str->text[str->text_len]), str2, str2_len);
	str->text_len += str2_len;
	return (SUCCESS);
}

/** Replaces the portion of the string that begins at character pos and spans len characters 
 * (or the part of the string in the range between [i1,i2)) by new contents: */
int	str_replace(t_string *str, size_t pos, size_t len, const char *str_to_replace)
{
	size_t	replace_len;
	char	*backup;
	size_t	i;

	if ((str_to_replace == NULL) || (pos > str->text_len - 1) || len == 0)
		return (NO_ACTION);
	replace_len = ft_strlen(str_to_replace);
	if (str->text_len + replace_len > str->capacity - 1 + len)
	{
		if (str_reserve(str, (str->capacity * 2) + replace_len) != SUCCESS)
			return (ERROR);
	}
	backup = NULL;
	if (pos <= str->text_len)
		backup = ft_strdup(&(str->text[pos + len]));
	i = str->text_len;
	while (i-- > pos)
		str_pop_back(str);
	if (pos <= str->text_len)
		str_append(str, str_to_replace);
	str_append(str, backup);
	if (backup != NULL)
		free(backup);
}

/** Edit string with all matches of a pattern replaced by a replacement 
 *  (ex. INPUT ["hi kyeu. hi again!"] 
 *  --> str_replace_all(str, "hi", "hello")
 *  --> OUTPUT["hello kyeu. hello again!")]
 * */
int	str_replace_all(t_string *str, const char *substr_old, const char *substr_new)
{
	// (1) 앞에서 부터 계속해서 찾는다. 
	


	// 만약 찾았다면, 그걸을 치환한다.
	


	// 만약 못찾았다면, 반복문을 종료한다.

}
