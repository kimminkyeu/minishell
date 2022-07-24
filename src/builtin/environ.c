/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 12:46:16 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/24 23:26:36 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environ.h"
#include "../main/helper.h"

/** 2차원 포인터 문자열들의 멤버 개수 확인 */
size_t	get_strs_count(char **strs)
{
	size_t	cnt;
	size_t	i;

	cnt = 0;
	i = 0;
	while (strs[i] != NULL)
	{
		cnt++;
		i++;
	}
	return (cnt);
}

void	copy_strs(char **dst, char **src, size_t strs_count)
{
	size_t	i;

	i = 0;
	while (i < strs_count)
	{
		dst[i] = ft_strdup(src[i]);
		i++;
	}
	dst[strs_count] = NULL;
}

char	**new_environ(char **system_envp)
{
	// (1) 환경변수 불러와서 깊은 복사 실행
	size_t	env_count;
	char	**new_envp;

	env_count = get_strs_count(system_envp);
	new_envp = ft_calloc(env_count + 1, sizeof(*new_envp));
	copy_strs(new_envp, system_envp, env_count);
	return (new_envp);
}

void	delete_strs(char ***strs)
{
	size_t	i;

	i = 0;
	while ((*strs)[i] != NULL)
	{
		free((*strs)[i]);
		(*strs)[i] = NULL;
		i++;
	}
	free(*strs);
	*strs = NULL;
}

void	sort_ascii(int len, char **argv)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = i;
		while (j + 1 < len)
		{
			if (ft_strncmp(argv[j], argv[j + 1], ft_strlen(argv[j] + 1)) > 0)
			{
				tmp = argv[j];
				argv[j] = argv[j + 1];
				argv[j + 1] = tmp;
			}
			++j;
		}
		--len;
	}
}

void	delete_environ(char ***our_envp)
{
	delete_strs(our_envp);
}

void	print_strs(char **strs)
{
	size_t	i;

	i = 0;
	while (strs[i] != NULL)
	{
		printf("%s\n", strs[i]);
		i++;
	}
}

void	print_in_order(char **our_envp)
{
	size_t	env_count;
	char	**ordered_envp;

	env_count = get_strs_count(our_envp);
	ordered_envp = ft_calloc(env_count + 1, sizeof(*ordered_envp));
	copy_strs(ordered_envp, our_envp, env_count);
	sort_ascii(env_count, ordered_envp);
	print_strs(ordered_envp);
	delete_strs(&ordered_envp);
}

void	add_to_envp(char *str, char ***our_envp_ptr)
{
	size_t	strs_count;
	char	**new_envp;

	strs_count = get_strs_count(*our_envp_ptr);
	new_envp = ft_calloc(strs_count + 2, sizeof(*new_envp));
	copy_strs(new_envp, *our_envp_ptr, strs_count);
	delete_environ(our_envp_ptr);
	new_envp[strs_count] = ft_strdup(str);
	new_envp[strs_count + 1] = NULL;
	*our_envp_ptr = new_envp;
	

	/** printf("\n\nadd to envp result\n\n"); */
	/** print_strs(*our_envp_ptr); */

}

/** 수정 후 코드 : 이미 등록된 애는 검색 후 수정하는 방식으로 ㄱ. */
int	exec_export(char **arglist, char ***our_envp_ptr)
{
	char	**tmp_split;
	char	*tmp;

	if (arglist[1] == NULL) // 인자가 없는 경우.
		print_in_order(*our_envp_ptr);
	else if (ft_isdigit(arglist[1][0]))
	{
		ft_putstr_fd("export: ", STDERR_FILENO);
		ft_putstr_fd(arglist[1], STDERR_FILENO);
		ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
		return (ERROR);
	}
	else
	{
		/** 이미 있는 환경변수일 경우 삭제 후 갱신. */
		tmp_split = ft_split(arglist[1], '=');
		tmp = arglist[1];
		arglist[1] = tmp_split[0];
		exec_unset(arglist, our_envp_ptr);
		arglist[1] = tmp;
		delete_strs(&tmp_split);
		add_to_envp(arglist[1], our_envp_ptr);
	}
	return (SUCCESS);
}



/** 수정 전 코드. 이미 밖에서 쿼트 제거를 하기 때문에 이건 틀림.  */
/** int	exec_export(char **arglist, char ***our_envp_ptr)
  * {
  *     char	**str_quote_split;
  *     char	*str_quote_removed;
  *     size_t	str_len;
  *
  *     if (arglist[1] == NULL) // 인자가 없는 경우.
  *         print_in_order(*our_envp_ptr);
  *     else
  *     {
  *         str_quote_split = NULL;
  *         str_quote_removed = NULL;
  *
  *         [> (1) remove quote <]
  *         str_len = ft_strlen(arglist[1]);
  *         if (arglist[1][str_len - 1] == '\"')
  *             str_quote_split = ft_split(arglist[1], '\"');
  *         else if (arglist[1][str_len - 1] == '\'')
  *             str_quote_split = ft_split(arglist[1], '\'');
  *
  *         if (str_quote_split != NULL)
  *         {
  *             str_quote_removed = ft_strjoin(str_quote_split[0], str_quote_split[1]);
  *             delete_strs(&str_quote_split);
  *         }
  *         else
  *             str_quote_removed = ft_strdup(arglist[1]);
  *
  *         [> (2) check string and add to envp <]
  *         if (ft_isdigit(str_quote_removed[0]))
  *         {
  *             ft_putstr_fd("export: ", STDERR_FILENO);
  *             ft_putstr_fd(str_quote_removed, STDERR_FILENO);
  *             ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
  *             free(str_quote_removed);
  *             return (ERROR);
  *         }
  *         else
  *             add_to_envp(str_quote_removed, our_envp_ptr);
  *     }
  *     return (SUCCESS);
  * }
  *  */

/** 정렬 없이 출력 진행. 그렇다고 삽입한 대로 출력되는건 아닌듯... */
int	exec_env(char **arglist, char **our_envp)
{
	if (arglist[0] == NULL)
		return (ERROR);
	if (arglist[1] != NULL && arglist[1][0] == '-')
	{
		ft_putstr_fd("env: illegal option or argument", STDERR_FILENO);
		ft_putstr_fd(arglist[1], STDERR_FILENO);
		return (ERROR);
	}
	else
		print_strs(our_envp);
	return (SUCCESS);
}

/** 환경 변수 리스트에서 제거 */
int	exec_unset(char **arglist, char ***our_envp_ptr)
{
	char	**str_split;
	char	**new_envp;
	size_t	strs_count;
	size_t	i;
	size_t	j;

	if (arglist[1] != NULL && arglist[1][0] == '-')
	{
		ft_putstr_fd("unset: illegal option ", STDERR_FILENO);
		ft_putstr_fd(arglist[1], STDERR_FILENO);
		return (ERROR);
	}
	else if (arglist[1] != NULL)
	{
		i = 0;
		j = i;
		strs_count = get_strs_count(*our_envp_ptr);
		new_envp = ft_calloc(strs_count + 1, sizeof(*new_envp));
		while (i < strs_count)
		{
			str_split = ft_split((*our_envp_ptr)[i], '=');
			if (ft_strncmp(str_split[0], arglist[1], ft_strlen(str_split[0]) + 1) != 0)
			{
				new_envp[j] = (*our_envp_ptr)[i];
				j++;
			}
			else
			{
				/** printf("found target : %s\n", arglist[1]); */
				free((*our_envp_ptr)[i]);
			}
			delete_strs(&str_split);
			i++;
		}
		free(*our_envp_ptr);
		*our_envp_ptr = new_envp;

		/** printf("\n"); */
		/** print_strs(*our_envp_ptr); */
		/** printf("\n"); */
        /**  */

	}
	return (SUCCESS);
}

char	*get_environ_value(const char *env_key, char **envp)
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
