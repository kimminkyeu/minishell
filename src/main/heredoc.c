#include <readline/readline.h>
#include <fcntl.h>
#include "minishell.h"
#include "heredoc.h"
#include "../lexer/token.h"

# define CHILD (0)

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
		while (1)
		{
			line = readline_prompt_heredoc();
			if (is_limiter(line, tok->str->text) == true)
				exit(SUCCESS);
			write(pipefd[WRITE], line, ft_strlen(line));
			write(pipefd[WRITE], "\n", 2);
			free(line);
		}
	}
	else
	{
		close(pipefd[WRITE]);
		if (wait(NULL) == -1)
			perror("wait fail");
		tok->heredoc_fd = pipefd[READ];
	}
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
