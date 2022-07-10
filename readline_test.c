/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 13:46:32 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/10 14:22:08 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** NOTE : to use readline lib, you must compile with -lreadline flag  */

int main(void)
{
	while (true)
	{
		char *line = readline("> ");

		// add history 를 하면 이전에 작성한 라인을 불러올 수 있다.
		add_history(line);

		printf("%s\n", line);
		
		// readline에서 받아온 한 줄은 반드시 free를 해줘야 한다. 
		free(line);
	}
	return (EXIT_SUCCESS);
}
