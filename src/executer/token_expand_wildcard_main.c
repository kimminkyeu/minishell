/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard_main.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yehan <yehan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 00:44:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/08/01 14:51:03 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h> // for opendir


#include <sys/stat.h> // for stat()


#include "token_expand.h"
#include "executer.h"
#include "../parser/parse_tree.h"

/**  
 * (1) if redirection : if matched string is more than one, then print "Ambiguous redirection"
 *
 * TODO : wildcard에서 정렬을 해줘야 하는가? --> 이 부분 꼭 체크!
 *
 * */


/** Stat을 써야 하는 이유..? : readdir에서 [.] 과 [..]을 걸러내야 한다...? */
/** struct stat	buf;
  * (stat(tmp->content, &buf) != 0) --> 0이면 [.][..]에 해당.
  *
  * 근데 굳이 써야 하나... */

/** 아 생각해보니 디렉토리인지 아닌지도 체크해야힘. 왜냐면 *//* 확장은 디렉토리만 이뤄지니까.

 따라서 로직은. *//*는 상관없이 앞부분 전부 확인하고, 만약 /가 포함되었다면 디렉토리만 확장하고 디렉토리가 아닌 애들은 전부 확장 리스트에서 삭제한다. 
*/

/** if redir, then prepare Ambiguous redirection.
 * else, if no wildcards to expand, return NULL */

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
	/** if (n == 0) */
		/** return (0); */
	/** else */
		return ((int)(*s1_uchar - *s2_uchar));
}
/** 경로를 체크해서 그에 맞는 경로 리스트를 반환 
 * mode(0) : default. 경로 든 아니든 모두 매칭
 * mode(1) : 디렉토리만 체크
 * mode(2) : 파일만 체크 */
t_list	*match_path_and_return_list(char *path, t_string *prefix, t_string *suffix, t_string *other)
{
	t_list			*match;
	DIR				*dir_ptr;
	struct dirent	*file;
	struct stat		buf;

	match = NULL;
	dir_ptr = opendir(path);
	if (dir_ptr == NULL)
		return (NULL);
	while (true)
	{
		file = readdir(dir_ptr);
		if (file == NULL)
			break ;

		/** 디렉토리 검색을 위한 stat사용. */
		stat(file->d_name, &buf);

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

t_list	*expand_single_wildcard(t_token *tok, t_shell_config *config)
{
	t_iterator iter;
	char	c;
	t_string *prefix;
	t_string *suffix;
	t_string *other;
	t_string *path;

	/** 패턴 정보 (1) */
	prefix = new_string(16);
	/** 패턴 정보 (2) */
	suffix = new_string(16);
	/** 패턴 제외한 남은 정보들. */
	other = new_string(16);
	/** 경로 정보.  */
	path = new_string(16);
	

	init_iterator(&iter, tok->str->text);

	/** 경로 정보. */
	path->f_append(path, get_environ_value("PWD", *config->envp));
	path->f_push_back(path, '/');

	// ls - s* 같은 경우 /가 NULL이기 때문에 경로정보는 없다고 본다. 
	// ls - src/* 같은 경우 /가 *앞에 있기 때문에 src/는 경로정보이다.
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
					/** path->f_push_back(path, iter.f_next(&iter)); */
					break ;
				}
				path->f_push_back(path, iter.f_next(&iter));
			}
		}

	
	int	flag = 0;
	while (iter.f_has_next(&iter))
	{
		// [ls]-[src/*]
		// [ls /*] [ls *] [ls */*]
		// [ls /] 절대경로로 뜸. 하면
		c = iter.f_peek(&iter);
		if (flag != 2 && c == '*')
		{
			flag = 1;
			iter.f_next(&iter);
		}
		if (c == '/')
		{
			flag = 2;
			other->f_push_back(other, iter.f_next(&iter));
		}
		else if (flag == 0)
			get_prefix(&iter, prefix);
		else if (flag == 1)
			get_suffix(&iter, suffix);
		else
			other->f_push_back(other, iter.f_next(&iter));
	}


	/** printf("path: [%s]\n", path->text); */
	/** printf("prefix: [%s]\n", prefix->text); */
	/** printf("suffix: [%s]\n", suffix->text); */
	/** printf("other: [%s]\n", other->text); */


	
	t_list *new;

	new = match_path_and_return_list(path->text, prefix, suffix, other);
		
	t_list *cur;
	t_token *tok2;

	if (path->text[path->text_len - 1] != '/')
		path->f_push_back(path, '|');
	cur = new;
	while (cur != NULL)
	{
		tok2 = cur->content;
		/** if (tok2->str->text[tok2->str->text_len - 1] != '/') */
			/** tok2->str->f_push_back(tok2->str, '/'); */
		tok2->str->f_insert(tok2->str, 0, ft_strrchr(path->text, '/') + 1);
		tok2->str->f_replace_all(tok2->str, "|", "/");

		/** tok2->str->f_insert(tok2->str, ft_strchr(path->text, '\0') - ft_strrchr(path->text, '/') - 1, "/"); */
		tok2->str->f_append(tok2->str, other->text);
		cur = cur->next;
	}
	/** printf("\nIn expand_single_wildcard\n"); */
	/** print_tokens(new); */


	delete_string(&prefix);
	delete_string(&suffix);
	delete_string(&other);
	delete_string(&path);

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


	/** Here */
	/** printf("\n\nhere in each token\n"); */
	/** print_tokens(cur_token); */
	/** printf("\n\n"); */


	expanded_token = NULL;
	tok = cur_token->content;
	redir_err_messege = ft_strdup(tok->str->text);

	/** 토큰 확장을 하고, *가 전체 리스트에서 없을 때 까지 재귀적으로 반복.  */
	expanded_token = expand_single_wildcard(tok, config);


	/** NOTE : 만약 리다이렉션에서 확장 후 여러개가 걸린다면. */
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

/** TODO : ["*"]는 확장이 되면 안되긴 함... 근데 이미 쿼트 리무벌이 됬기 때문에 그걸 감지 못함.
 * 이거 참 골치아픔. */
int	expand_wildcard_glob_once(t_list *tokens, t_shell_config *config)
{
	t_list	*cur;
	t_list	*tmp;
	t_list	*tmp2;

	int		is_error;

	/** 못찾을 경우 복구용. */

	is_error = false;
	cur = tokens;
	if (cur == NULL || cur->next == NULL)
		return (SUCCESS);

	while (cur != NULL && cur->next != NULL)
	{
		if (has_wild_card(cur->next))
		{
			tmp = cur->next->next;
			tmp2 = expand_wildcard_glob_and_return_list(cur->next, config, &is_error);
			if (tmp2 != NULL)
			{
				ft_lstdelone(cur->next, delete_token);
				cur->next = tmp2;

			}
			/** 만약 못찾았다면 단순 연결 복구 */
			else if (tmp2 == NULL)
				cur->next = tmp;

			/** if (is_error == true) // ambiguous redirection error */
				/** return (ERROR); */
			while (cur != NULL && cur->next != NULL)
			{
				tmp2 = cur;
				cur = cur->next;
			}
			cur->next = tmp;
		}
		else
		{
			tmp2 = cur;
			cur = cur->next;
		}
	}
	/** printf("\n\ncheck\n"); */
	/** print_tokens(tmp2); */

	if (tmp2 != NULL && tokens->next != cur && has_wild_card(cur))
	{
		/** printf("\n\n------------------------------------\n\n"); */
		tmp = cur; // 마지막 노드
		tmp2->next = expand_wildcard_glob_and_return_list(cur, config, &is_error);
		if (tmp2->next != NULL)
		{
			ft_lstdelone(tmp, delete_token);
		}
		/** else if (tmp2->next == NULL) */
			/** tmp2->next = cur; */
	}


	if (is_error == true) // ambiguous redirection error
		return (ERROR);

	return (SUCCESS);
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

	tok = NULL;
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
		status = expand_wildcard_glob_once(tokens, config);
	}
	if (has_wild_card == true && tokens->next == NULL)
		tokens->next = ft_lstnew(new_token(before_expansion));
	return (status);
}
