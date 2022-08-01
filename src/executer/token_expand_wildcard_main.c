/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard_main.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 00:44:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/01 20:25:36 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "token_expand.h"
#include "executer.h"
#include "../parser/parse_tree.h"

void	get_prefix(t_iterator *iter, t_string *prefix)
{
	int	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_peek(iter);
		if (c == '*')
			break ;
		else
			prefix->f_push_back(prefix, iter->f_next(iter));
	}
}

void	get_suffix(t_iterator *iter, t_string *suffix)
{
	int	c;

	while (iter->f_has_next(iter))
	{
		c = iter->f_peek(iter);
		if (c == '/')
			break ;
		else
			suffix->f_push_back(suffix, iter->f_next(iter));
	}
}

int	ft_strncmp_reverse(const char *s1, const char *s2, size_t n)
{
	char	*s1_uchar;
	char	*s2_uchar;

	s1_uchar = ft_strrchr(s1, '\0');
	s2_uchar = ft_strrchr(s2, '\0');
	while (s1_uchar >= s1 && s2_uchar >= s2 && (*s1_uchar == *s2_uchar) && n > 0)
	{
		s1_uchar--;
		s2_uchar--;
		n--;
	}
	return ((int)(*s1_uchar - *s2_uchar));
}

t_list	*match_path_and_return_list(char *path, t_string *prefix, t_string *suffix, t_string *other)
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

		/** stat(file->d_name, &buf); */
		/** 디렉토리 검색을 위한 stat사용. */
		t_string	*path_joined = new_string(64);

		path_joined->f_append(path_joined, path);
		if (path_joined->text[path_joined->text_len - 1] != '/')
			path_joined->f_push_back(path_joined, '/');
		path_joined->f_append(path_joined, file->d_name);

		/** printf("path in stat():[%s]\n", path_joined->text); */
		stat(path_joined->text, &buf);
		delete_string(&path_joined);

		/** printf("file [%s]\n", file->d_name); */

		/** 만약 prefix 혹은 suffix가 빈문자열이면 비교검사 할 필요 없음. + .. 과 .는 제외  */
		if (prefix->f_is_empty(prefix) && suffix->f_is_empty(suffix))
		{
			/** printf("other->text[0]:%c\t\tfile_name[%s]\t\tS_ISDIR:%d\n",other->text[0], file->d_name, S_ISDIR(buf.st_mode)); */
			/** 모든 와일드카드에서 .. 와 .는 제외*/
			if (ft_strncmp(file->d_name, "..", 2) != 0 && ft_strncmp(file->d_name, ".", 1) != 0)
			{
				/** other가 /라면 경로에 해당하는 것만 리스트에 추가.  */
				if (other->text[0] == '/' && S_ISDIR(buf.st_mode))
				{
					/** printf("file push to list: %s\n", file->d_name); */
					ft_lstadd_back(&match, ft_lstnew(new_token(file->d_name)));
				}
				else if (other->text[0] != '/')
				{
					ft_lstadd_back(&match, ft_lstnew(new_token(file->d_name)));
				}
			}
		}
		/** prefix와 suffix가 빈 문자열이 아니면 검색 시작.  */
		else if (ft_strncmp(file->d_name, prefix->text, prefix->text_len) == 0 \
				&& ft_strncmp_reverse(file->d_name, suffix->text, suffix->text_len) == 0)
		{
			/** printf("other->text[0]:%c\t\tfile_name[%s]\t\tS_ISDIR:%d\n",other->text[0], file->d_name, S_ISDIR(buf.st_mode)); */
			if (other->text[0] == '/' && S_ISDIR(buf.st_mode))
			{
				ft_lstadd_back(&match, ft_lstnew(new_token(file->d_name)));
			}
			else if (other->text[0] != '/')
				ft_lstadd_back(&match, ft_lstnew(new_token(file->d_name)));
		}
	}


	closedir(dir_ptr);
	return (match);
}

void	init_pattern(t_pattern *pattern)
{
	pattern->path = new_string(32);
	pattern->prefix = new_string(32);
	pattern->suffix = new_string(32);
	pattern->remainder = new_string(32);
}

void	delete_pattern(t_pattern *pattern)
{
	delete_string(&pattern->path);
	delete_string(&pattern->prefix);
	delete_string(&pattern->suffix);
	delete_string(&pattern->remainder);
}

void	set_pattern(t_token *tok, t_pattern *pattern, t_shell_config *config)
{
	int			flag;
	t_iterator	iter;
	char		c;

	flag = 0;
	init_iterator(&iter, tok->str->text);
	pattern->path->f_append(pattern->path, get_environ_value("PWD", *config->envp));
	pattern->path->f_push_back(pattern->path, '/');
	if (ft_strchr(tok->str->text, '/') != NULL && ft_strchr(tok->str->text, '*') != NULL)
		if (ft_strchr(tok->str->text, '/') < ft_strchr(tok->str->text, '*'))
		{
			while (iter.f_has_next(&iter))
			{
				c = iter.f_peek(&iter);
				if (c == '*')
					break ;
				if (c == '/')
				{
					iter.f_next(&iter);
					break ;
				}
				pattern->path->f_push_back(pattern->path, iter.f_next(&iter));
			}
		}
	while (iter.f_has_next(&iter))
	{
		c = iter.f_peek(&iter);
		if (flag != 2 && c == '*')
		{
			flag = 1;
			iter.f_next(&iter);
		}
		if (c == '/')
		{
			flag = 2;
			pattern->remainder->f_push_back(pattern->remainder, iter.f_next(&iter));
		}
		else if (flag == 0)
			get_prefix(&iter, pattern->prefix);
		else if (flag == 1)
			get_suffix(&iter, pattern->suffix);
		else
			pattern->remainder->f_push_back(pattern->remainder, iter.f_next(&iter));
	}
}












t_list	*expand_single_wildcard(t_token *tok, t_shell_config *config)
{
	t_pattern	pattern;
	t_list *new;
	t_list *cur;
	t_token *tok2;

	init_pattern(&pattern);
	set_pattern(tok, &pattern, config);
	new = match_path_and_return_list(pattern.path->text, pattern.prefix, pattern.suffix, pattern.remainder);
	if (pattern.path->text[pattern.path->text_len - 1] != '/')
		pattern.path->f_push_back(pattern.path, '|');
	cur = new;
	while (cur != NULL)
	{
		tok2 = cur->content;
		tok2->str->f_insert(tok2->str, 0, ft_strrchr(pattern.path->text, '/') + 1);
		tok2->str->f_replace_all(tok2->str, "|", "/");
		tok2->str->f_append(tok2->str, pattern.remainder->text);
		cur = cur->next;
	}
	delete_pattern(&pattern);
	return (new);
}

int	has_wild_card(t_list *cur_token)
{
	t_token *tok;

	if (cur_token == NULL)
		return (false);
	tok = cur_token->content;
	if (ft_strchr(tok->str->text, '*'))
		return (true);
	else
		return (false);
}

t_list	*expand_wildcard_glob_and_return_list(t_list *cur_token, t_shell_config *config, int *is_error)
{
	t_token	*tok;
	t_list	*expanded_token;
	char	*redir_err_messege;
	
	if (cur_token == NULL)
		return (NULL);
	expanded_token = NULL;
	tok = cur_token->content;
	redir_err_messege = ft_strdup(tok->str->text);
	expanded_token = expand_single_wildcard(tok, config);
	if ((tok->type >= 5 && tok->type <= 9) || tok->type == 13)
		if (expanded_token != NULL && expanded_token->next != NULL)
		{
			printf("lesh: %s: ambiguous redirect\n", redir_err_messege);
			ft_lstclear(&expanded_token, delete_token);
			expanded_token = NULL;
			*is_error = true;
		}
	free(redir_err_messege);
	return (expanded_token);
}

int	expand_wildcard_glob_once(t_list *cur, t_shell_config *config, int *is_error)
{
	t_list	*tmp;
	t_list	*expanded_list;
	t_list	*prev;

	while (cur != NULL && cur->next != NULL)
	{
		if (has_wild_card(cur->next))
		{
			tmp = cur->next->next;
			prev = cur->next;
			expanded_list = expand_wildcard_glob_and_return_list(cur->next, config, is_error);
			if (expanded_list != NULL)
			{
				cur->next = expanded_list;
				while (cur != NULL && cur->next != NULL)
					cur = cur->next;
			}
			cur->next = tmp;
			ft_lstdelone(prev, delete_token);
		}
		cur = cur->next;
	}
	return (*is_error);
}

int	has_wild_card_in_list(t_list *tokens)
{
	t_list	*cur;

	cur = tokens;
	while (cur != NULL)
	{
		if (has_wild_card(cur))
			return (true);
		cur = cur->next;
	}
	return (false);
}

int	expand_wildcard_glob(t_list *tokens, t_shell_config *config)
{
	int	status;
	int	has_wild_card;
	t_token	*tok;
	char	*before_expansion;
	int		is_error;

	tok = NULL;
	is_error = false;
	before_expansion = NULL;
	if (tokens != NULL && tokens->next != NULL)
	{
		tok = tokens->next->content;
		before_expansion = ft_strdup(tok->str->text);
	}
	status = SUCCESS;
	has_wild_card = false;
	while (tokens != NULL && tokens->next != NULL && status == SUCCESS && has_wild_card_in_list(tokens))
	{
		has_wild_card = true;
		status = expand_wildcard_glob_once(tokens, config, &is_error);
	}
	if (has_wild_card == true && tokens->next == NULL)
		tokens->next = ft_lstnew(new_token(before_expansion));
	return (status);
}
