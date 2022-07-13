/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:03:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 22:09:36 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "scanner.h"
#include "iterator.h"

/** NOTE : std::string =================================== */

typedef struct s_string t_string;

typedef struct s_string {
	char		*text;
	size_t		text_len;
	size_t		capacity;
}	t_string;

void		*string_push_back(t_string *str, char c)
{
	char	*new_text;

	if (str->text_len >= str->capacity - 1)
	{
		new_text = ft_calloc(str->capacity * 2, sizeof(*(str->text)));
		if (new_text == NULL)
			return (NULL);
		ft_memmove(new_text, str->text, str->text_len);
		str->text = new_text;
	}
	str->text_len++;
	str->text[str->text_len] = c;
	return (str);
}

void		*string_pop_back(t_string *str)
{
	if (str->text_len > 0)
	{
		str->text[str->text_len - 1] = '\0';
		str->text_len--;
		return (str);
	}
	return (NULL);
}

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
}

void	delete_string(t_string **address_of_string)
{
	free((*address_of_string)->text);
	free(*address_of_string);
	address_of_string = NULL;
}


/** ====================================================== */




/** if full, then realloc token */
void	add_char_to_token(char c)
{
	

}


/** Free allocated token */
void	free_token(t_token *token)
{
	if (token->text != NULL)
		free(token->text);
	free(token);
}


/* Create single token datastructure (malloc) */
t_token	*new_token_malloc(char *str)
{
	t_token *token;

	token = ft_calloc(1, sizeof(*token));
	token->text_len = ft_strlen(str);
	token->text = ft_calloc(1, token->text_len + 1);
	if (token->text == NULL)
		return (NULL);
	ft_memmove(token->text, str, token->text_len);
	return (token);
}

/**  */
void tokenize(char *str, t_vector *token)
{

}
