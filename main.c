/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 14:31:42 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/08 14:52:43 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

typedef struct s_token {


};

typedef struct s_syntax_tree {
	// ...
}	t_syntax_tree;

// TODO: libft 파일 여기로 집어넣기.

void	shell_loop(void)
{
	char			*cmd_line;
	t_syntax_tree	**cmd_tree;
	int				status;

	status = 1;
	while (status == 1)
	{
		// (0) print initial message of shell.
		printf("> ");

		// (1) read cmd from STDIN. simple function.
		cmd_line = shell_read_line();

		// (2) tokenize + lexer + parser function
		cmd_tree = shell_parse_line(cmd_line);

		// (3) execute cmd, then return it's status to close shell
		status = shell_execute(cmd_tree);

		// (4) free data
		free(cmd_line);
		free(cmd_tree);
	}
}

int main(int ac, char **av)
{
	// load config files

	// if there is a script, then handle < bash ./my_bash_script > format

	// handle < bash my_script.sh | echo new -> print $new

	// bash my_script.sh hahaha --> 에러 가 아닌 my_script.sh가 스크립트로 읽혀야한다.
	// 근데 이걸 구현할 필요가 있을까? 꼭? 일단 나중에 해보자.

	// run command loop
	shell_loop();

	// Perform any shutdown/clean up
}