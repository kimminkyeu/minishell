/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 09:59:33 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/13 17:33:07 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "scanner.h"
#include "token.h"

/* Test for lexical analizer */
int	main(int ac, char **av)
{
	t_list	*tokens;

	if (ac != 2)
		return (EXIT_FAILURE);

	/** (1) get token list from tokenizer */
	tokens = tokenize(av[1]);


	/** (2) check token error (raw syntax cheking) */
	// ...


	/** (3) print token list */
	// ...


	/** (4) free token list */
	ft_lstclear(&tokens, delete_token);
	

	return (0);
}
