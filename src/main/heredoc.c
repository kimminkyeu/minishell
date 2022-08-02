/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:20:24 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 13:29:01 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <stdio.h>
#include <readline/readline.h>
#include <fcntl.h>
#include <signal.h>

#include "minishell.h"
#include "heredoc.h"
#include "../lexer/token.h"

#define CHILD (0)

extern int	g_is_sig_interupt;

void	expand_token(t_token *tok)
{
	t_iterator	iter;
	t_string	*expanded_str;
	char		c;

	init_iterator(&iter, tok->str->text);
	expanded_str = new_string(tok->str->capacity);
	while (iter.f_has_next(&iter))
	{
		c = iter.f_next(&iter);
		if (c == '\'' || c == '\"')
		{
			expand_quote(expanded_str, &iter, c);
			tok->type = E_TYPE_REDIR_ARG_HEREDOC_QUOTED;
		}
		else
			expanded_str->f_push_back(expanded_str, c);
	}
	delete_string(&tok->str);
	tok->str = expanded_str;
}

void	open_heredoc(t_token *tok)
{
	int		pipefd[2];
	pid_t	pid;
	char	*line;

	pipe(pipefd);
	pid = fork();
	if (pid == CHILD)
	{
		close(pipefd[READ]);
		while (true)
		{
			line = readline_prompt_heredoc();
			/** line = readline("> "); */
			if (is_limiter(line, tok->str->text) == true)
				exit(SUCCESS);
			write(pipefd[WRITE], line, ft_strlen(line));
			write(pipefd[WRITE], "\n", 2);
			free(line);


			/** if (g_is_sig_interupt == true) */
			/** { */
			/**     [> printf("Exit\n"); <] */
			/**     exit(SUCCESS); */
			/** } */
		}
		/** g_is_sig_interupt = false; */
		exit(10);
	}
	else
	{
		int w_status;	

		while (true)
		{
			int ret = waitpid(pid, &w_status, WNOHANG);
			if (ret == 0)
			{
				if (WEXITSTATUS(w_status) == 10 || g_is_sig_interupt == true)
				{
					kill(pid, SIGTERM);
					break ;
				}
			}
			else
				return ;
		}
			/** perror("wait fail"); */

		/** g_is_sig_interupt = false; */
		/**  */

		/** printf("g_is_sig_interupt in parent[%d]\n", g_is_sig_interupt); */

		/** printf("parent waiting...\n"); */
		/** while (g_is_sig_interupt == false) */
		/** { */
			/** if (g_is_sig_interupt == true) */
				/** kill(pid, SIGTERM); */
			/** printf("heredoc_sig_catched\n"); */
			/** g_is_sig_interupt = false; */
			/** return ; */
		/** } */
	}
	close(pipefd[WRITE]);
	tok->heredoc_fd = pipefd[READ];
}

void	set_heredoc(t_list *tokens)
{
	t_list	*cur;
	t_token	*tok;

	cur = tokens;
	while (cur)
	{
		tok = cur->content;
		if (tok->type == E_TYPE_REDIR_HEREDOC)
		{
			cur = cur->next;
			tok = cur->content;
			expand_token(tok);
			open_heredoc(tok);
		}
		cur = cur->next;
	}
}
