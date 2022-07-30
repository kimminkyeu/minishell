/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand_wildcard_main.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 00:44:13 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/31 02:30:29 by minkyeki         ###   ########.fr       */
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

void	find_path(char *path)
{
	DIR				*dir_ptr;
	struct dirent	*file;
	/** struct stat		buf; */

	dir_ptr = opendir(path);
	if (dir_ptr == NULL)
		return ;

	while (true)
	{
		file = readdir(dir_ptr);
		if (file == NULL)
			break ;
		printf("%s\n", file->d_name);
	}
	closedir(dir_ptr);
}

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
t_list	*expand_wildcard_glob_and_return_list(t_list *cur_token, t_shell_config *config)
{
	t_token	*tok;
	t_list	*expanded_tokens;
	
	expanded_tokens = NULL;
	tok = cur_token->content;

	/** if Type REDIRECT 
	 * FIXME : 이걸 여기서 검사하면 안된다. 로직상 함께 처리해야 함. 하면서 검사. */
	/** if ((tok->type >= 5 && tok->type <= 9) || tok->type == 13) */
	t_iterator iter;
	char	c;
	
	/** [asdasf*ad.c]
	 * prefix = adsasf
	 * suffix = ad.c
	 * 
	 *  [.*]
	 *  prefix = .
	 *  suffix = NULL
	 *
	 * */

	/** 패턴 정보 (1) */
	t_string *prefix;
	prefix = new_string(16);

	/** 패턴 정보 (2) */
	t_string *suffix;
	suffix = new_string(16);

	init_iterator(&iter, tok->str->text);
	while (iter.f_has_next(&iter))
	{
		int	flag = 0;
		/** [ls .*] [ls *] [ls *//*] */
		/** [ls //] 절대경로로 뜸. 하면  */
		c = iter.f_next(&iter);
		if (c == '*')
			flag = 1;
		else if (flag == 1 && c == '/')
		{
			// 

		}
		else if (flag == 1)
			suffix->f_push_back(suffix, c);
		else
			prefix->f_push_back(prefix, c);
	}

	find_path(get_environ_value("PWD", *config->envp));

	return (expanded_tokens);
}

void	expand_wildcard_glob(t_list *tokens, t_shell_config *config)
{
	t_list	*cur;
	t_list	*prev;

	prev = NULL;
	cur = tokens;
	while (cur != NULL)
	{
		/** expand list */
		if (prev == NULL) // if first node
		{
			/** 고민 아 어떻게 앞에다가 이걸 노드를 추가하지... 요기다 주소 넣으면 터질텐데.  */
			ft_lstadd_front(&tokens, ...)

		}

		if (cur != NULL)
		{
			cur = expand_wildcard_glob_and_return_list(cur, config);
			while (cur->next != NULL)
				cur = cur->next;
			cur->next = tmp;
		}


		cur = cur->next;
	}
}

t_list	*ft_lst_get_prev_node(t_list *node, t_list *target)




