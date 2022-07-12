/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 10:02:06 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/12 10:05:05 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>


int main(void)
{
	char	*line;

	while (true)
	{
		line = readline("> ");

		// add history 를 하면 이전에 작성한 라인을 불러올 수 있다.
		add_history(line);
		printf("input : %s\n", line);
		


		// readline에서 받아온 한 줄은 반드시 free를 해줘야 한다. 
		free(line);
	}
	return (EXIT_SUCCESS);
}
