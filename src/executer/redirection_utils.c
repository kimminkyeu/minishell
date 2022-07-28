#include <stdio.h>

#include "../main/minishell.h"
#include "../lexer/token.h"

# define CHILD	(0)

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

char	*expand_line_each(char *line, t_shell_config *config)
{
	t_iterator	iter;
	t_string	*expanded_str;
	char		c;
	char		*newline;

	init_iterator(&iter, line);
	expanded_str = new_string(50);
	while (iter.f_has_next(&iter))
	{
		c = iter.f_next(&iter);
		if (c == '$')
			expand_dollar_sign(expanded_str, &iter, config);
		else
			expanded_str->f_push_back(expanded_str, c);
	}
	newline = ft_strdup(expanded_str->text);
	delete_string(&expanded_str);
	return (newline);
}

void	expand_file(t_token *tok, t_shell_config *config)
{
	char	*line;
	char	*newline;
	pid_t	pid;
	int		pipefd[2];

	//0. token->heredoc_fd에 기존 fd 있음.

	// 1. 새 파이프를 하나 열고
	if (pipe(pipefd) == -1)
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
				exit(SUCCESS);
			newline = expand_line_each(line, config);
			free(line);
			write(pipefd[WRITE], newline, ft_strlen(newline));
			write(pipefd[WRITE], "\n", 2);
			free(newline);
		}
	}
	else//if parent
	{
		// 3. 새 파이프 read 저장
		close(pipefd[WRITE]);
		wait(NULL);
		close(tok->heredoc_fd);
		tok->heredoc_fd = pipefd[READ];
	}
}
