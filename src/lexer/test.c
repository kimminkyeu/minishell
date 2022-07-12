/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 09:59:33 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 13:25:24 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "scanner.h"

/* Test for lexical analizer */
void	main(int ac, char **av)
{
	t_vector *tokens;

	if (ac != 2)
		return (EXIT_FAILURE);
	
	tokens = tokenize(av[1]);

	int i = 0;
	int j = 0;
	while (i < tokens->size)
	{
		t_token *token = tokens->data[i];
		printf("[%d]", token->id);
		while (j < token->len)
		{
			printf("%c", token->str[j]);
			j++;
		}
		printf("\n");
		i++;
	}
}