/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard_pattern.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 21:11:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/02 15:43:47 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_expand_wildcard.h"

void	set_path_and_run_stat(struct dirent *file, char *path, struct stat *buf)
{
	t_string	*path_joined;

	path_joined = new_string(64);
	path_joined->f_append(path_joined, path);
	if (path_joined->text[path_joined->text_len - 1] != '/')
		path_joined->f_push_back(path_joined, '/');
	path_joined->f_append(path_joined, file->d_name);
	stat(path_joined->text, buf);
	delete_string(&path_joined);
}

void	match_pattern(struct dirent *file, struct stat *buf, \
		t_list **match, t_pattern *pattern)
{
	if (pattern->prefix->f_is_empty(pattern->prefix) \
			&& pattern->suffix->f_is_empty(pattern->suffix))
	{
		if (ft_strncmp(file->d_name, "..", 2) != 0 \
				&& ft_strncmp(file->d_name, ".", 1) != 0)
		{
			if (pattern->remainder->text[0] == '/' && S_ISDIR(buf->st_mode))
				ft_lstadd_back(match, ft_lstnew(new_token(file->d_name)));
			else if (pattern->remainder->text[0] != '/')
				ft_lstadd_back(match, ft_lstnew(new_token(file->d_name)));
		}
	}
	else if (ft_strncmp(file->d_name, pattern->prefix->text, \
				pattern->prefix->text_len) == 0 \
			&& ft_strncmp_reverse(file->d_name, pattern->suffix->text, \
				pattern->suffix->text_len) == 0)
	{
		if (pattern->remainder->text[0] == '/' && S_ISDIR(buf->st_mode))
			ft_lstadd_back(match, ft_lstnew(new_token(file->d_name)));
		else if (pattern->remainder->text[0] != '/')
			ft_lstadd_back(match, ft_lstnew(new_token(file->d_name)));
	}
}

t_list	*match_path_and_return_list(char *path, t_pattern *pattern)
{
	t_list			*match;
	DIR				*dir_ptr;
	struct dirent	*file;
	struct stat		buf;

	match = NULL;
	dir_ptr = opendir(path);
	if (path == NULL && dir_ptr == NULL)
		return (NULL);
	while (true)
	{
		file = readdir(dir_ptr);
		if (file == NULL)
			break ;
		set_path_and_run_stat(file, path, &buf);
		match_pattern(file, &buf, &match, pattern);
	}
	closedir(dir_ptr);
	return (match);
}

void	set_pattern_path(t_token *tok, t_pattern *pattern, t_iterator *iter)
{
	char	c;

	if (ft_strchr(tok->str->text, '/') != NULL \
			&& ft_strchr(tok->str->text, '*') != NULL)
	{
		if (ft_strchr(tok->str->text, '/') < ft_strchr(tok->str->text, '*'))
		{
			while (iter->f_has_next(iter))
			{
				c = iter->f_peek(iter);
				if (c == '*')
					break ;
				if (c == '/')
				{
					iter->f_next(iter);
					break ;
				}
				pattern->path->f_push_back(pattern->path, iter->f_next(iter));
			}
		}
	}
}

void	set_pattern_prefix_suffix_remainder(t_pattern *pattern, \
		t_iterator *iter)
{
	char	c;
	int		flag;

	flag = 0;
	while (iter->f_has_next(iter))
	{
		c = iter->f_peek(iter);
		if (flag != 2 && c == '*')
		{
			flag = 1;
			iter->f_next(iter);
		}
		if (c == '/')
			flag = 2;
		if (flag == 0)
			get_prefix(iter, pattern->prefix);
		else if (flag == 1)
			get_suffix(iter, pattern->suffix);
		else
			pattern->remainder->f_push_back(pattern->remainder, \
					iter->f_next(iter));
	}
}
