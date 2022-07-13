/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 09:59:33 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 21:22:26 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "scanner.h"

/* Test for lexical analizer */
int	main(int ac, char **av)
{
	t_vector *tokens;

	if (ac != 2)
		return (EXIT_FAILURE);

	tokens = new_vector(64);
	tokenize(av[1], tokens);

	int i = 0;
	int j = 0;
	while (i < tokens->size)
	{
		t_token *token = tokens->data[i];
		printf("[%d]", token->type);
		while (j < token->text_len)
		{
			printf("%c", token->text[j]);
			j++;
		}
		printf("\n");
		i++;
	}

	delete_vector(&tokens);
	return (0);
}
