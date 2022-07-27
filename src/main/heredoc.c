#include <readline/readline.h>
#include <fcntl.h>
#include "minishell.h"
#include "../lexer/token.h"

# define CHILD (0)

static void	expand_quote(t_string *str, t_iterator *iter, char quote_type)
{
	char	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_next(iter);
		if (c == quote_type)
			return ;
		else
			str->f_push_back(str, c);
	}
}

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

/*********************************************/

static bool	is_limiter(const char *line, const char *limiter)
{
	if (ft_strlen(line) == ft_strlen(limiter)
		&& !ft_strncmp(line, limiter, ft_strlen(limiter)))
		return (true);
	else
		return (false);
}

static char	*readline_prompt_heredoc(void)
{
	t_string	*prompt;
	static char	*line;

	prompt = new_string(64);
	prompt->f_append(prompt, "\033[31m");
	prompt->f_append(prompt, "heredoc");
	prompt->f_append(prompt, "\033[0m");
	prompt->f_append(prompt, "$ ");
	line = readline(prompt->text);
	delete_string(&prompt);
	return (line);
}

void	open_heredoc(t_token *tok, int num)
{
	int		pipefd[2];
	pid_t	pid;
	char	*line;

	(void)num;
	pipe(pipefd);
	pid = fork();
	if (pid == CHILD)
	{
		close(pipefd[READ]);
		while (1)
		{
			line = readline_prompt_heredoc();
			if (is_limiter(line, tok->str->text) == true)
				break ;
			write(pipefd[WRITE], line, ft_strlen(line));
			write(pipefd[WRITE], "\n", 2);
			free(line);
		}
	}
	else
	{
		close(pipefd[WRITE]);
		tok->heredoc_fd = pipefd[READ];
	}
}

/*************************************************/

void	set_heredoc(t_list *tokens)
{
	t_list	*cur;
	t_token	*tok;
	int		num;

	num = 1;
	cur = tokens;
	while (cur)
	{
		tok = cur->content;
		if (tok->type == E_TYPE_REDIR_HEREDOC)
		{
			cur = cur->next;
			tok = cur->content;
			expand_token(tok);
			open_heredoc(tok, num);
			num++;
		}
		cur = cur->next;
	}
}

/***************************************/
