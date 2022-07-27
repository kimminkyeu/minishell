/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: han-yeseul <han-yeseul@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 15:34:25 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 13:19:11 by han-yeseul       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/** Readline library */
#include <readline/readline.h>
#include <readline/history.h>

#include <fcntl.h>//open
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>//strerror

#include "../main/minishell.h"
#include "../lexer/token.h"

# define IN		(0)
# define OUT	(1)
# define CHILD	(0)

# define READ	(0)
# define WRITE	(1)


int	open_file_less(t_list *cur, int *pipe_fd, int *status)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;
	pipe_fd[READ] = open(tok->str->text, O_RDONLY);
	if (pipe_fd[READ] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

/******** heredoc ***********/


static char	*get_environ_value(const char *env_key, char **envp)
{
	size_t	i;
	char	*target;

	i = 0;
	if (env_key == NULL || envp == NULL)
		return (NULL);
	target = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], env_key, ft_strlen(env_key)) == 0)
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return (target);
}

static void	expand_dollar_sign(t_string *str, t_iterator *iter, t_shell_config *config)
{
	long	start;
	long	end;
	char	*env_key;
	char	*env_value;

	start = iter->curpos;
	while (iter->f_has_next(iter) && ft_isalnum(iter->f_peek(iter)))
		iter->f_next(iter);
	end = iter->curpos;
	env_key = ft_substr(iter->line, start + 1, end - start);
	env_value = get_environ_value(env_key, *(config->envp));
	str->f_append(str, env_value);
	free(env_key);
}

static void	expand_line_each(char *line, t_shell_config *config)
{
	t_iterator	iter;
	t_string	*expanded_str;
	char		c;

	init_iterator(&iter, line);
	expanded_str = new_string(ft_strlen(line) * 2);
	while (iter.f_has_next(&iter))
	{
		c = iter.f_next(&iter);
		if (c == '$')
			expand_dollar_sign(expanded_str, &iter, config);
		else
			expanded_str->f_push_back(expanded_str, c);
	}
	free(line);
	line = ft_strdup(expanded_str->text);
	delete_string(expanded_str);
}

static void	expand_file(t_token *tok, t_shell_config *config)
{
	int		fd_old;
	char	*line;
	pid_t	pid;
	int		pipefd[2];

	//token->heredoc_fd에 기존 fd 있음.

	// 1. 새 파이프를 하나 열고
	if (pipe(pipefd[2]) == -1)
		perror("pipe fail");
	pid = fork();
	if (pid == CHILD)
	{
		close(pipefd[READ]);
		// 2. 한 줄씩 읽어오며 $ 확장해서 새 파이프에 써넣기
		while (1)
		{
			line = get_next_line(tok->heredoc_fd);
			if (line == NULL)
				break ;
			expand_line_each(line, config);
			write(pipefd[WRITE], line, ft_strlen(line));
			free(line);
		}
	}
	// 3. 새 파이프 read 저장
	close(tok->heredoc_fd);
	close(pipefd[WRITE]);
	tok->heredoc_fd = pipefd[READ];
}

int	open_file_heredoc(t_list *cur, int *pipe_fd, int *status, t_shell_config *config)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;

	if (tok->type == E_TYPE_REDIR_ARG_HEREDOC_QUOTED)
		expand_file(tok, config);
	pipe_fd[READ] = tok->heredoc_fd;
	if (pipe_fd[READ] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

/************** heredoc end *******************/

int	open_file_greater(t_list *cur, int *pipe_fd, int *status)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;
	pipe_fd[WRITE] = open(tok->str->text, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (pipe_fd[WRITE] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

int	open_file_append(t_list *cur, int *pipe_fd, int *status)
{
	t_token	*tok;

	cur = cur->next;
	tok = cur->content;
	pipe_fd[WRITE] = open(tok->str->text, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (pipe_fd[WRITE] == -1)
	{
		*status = errno;
		printf("lesh: %s: %s\n", tok->str->text, strerror(*status));
	}
	return (*status);
}

int	open_redirection(int *pipe_fd, t_list *redir_list, t_shell_config *config)
{
	int				status;
	t_token			*tok;
	t_list			*cur;

	status = 0;
	if (redir_list == NULL)
		return (status);
	cur = redir_list;
	while (cur != NULL)
	{
		tok = cur->content;
		if (tok->type == E_TYPE_REDIR_LESS)//<in
			if (open_file_less(cur, pipe_fd, &status) != SUCCESS)
				break ;
		else if (tok->type == E_TYPE_REDIR_HEREDOC)//<<in
			if (open_file_heredoc(cur, pipe_fd, &status, config) != SUCCESS)
				break ;
		else if (tok->type == E_TYPE_REDIR_GREATER) // > out
			if (open_file_greater(cur, pipe_fd, &status) != SUCCESS)
				break ;
		else if (tok->type == E_TYPE_REDIR_APPEND) // >> out
			if (open_file_append(cur, pipe_fd, &status) != SUCCESS)
				break ;
		cur = cur->next;
	}
	return (status);
}
