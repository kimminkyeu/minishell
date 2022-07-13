/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 13:03:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/13 12:27:37 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "scanner.h"
#include "iterator.h"
#include "string.h"

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
