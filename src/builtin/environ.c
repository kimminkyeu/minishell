/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 12:46:16 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/20 21:57:42 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environ.h"

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
	while (*strs != NULL)
	{
		printf("%s\n", *strs);
		strs++;
	}
}

void	print_in_order(char **our_envp)
{
	size_t	env_count;

	/** FIXME : 이걸 static으로 하면, 출력할때마다 정렬하는 행위를 안하지 않을까?  */
	char	**ordered_envp;

	env_count = get_strs_count(our_envp);
	ordered_envp = ft_calloc(env_count + 1, sizeof(*ordered_envp));
	copy_strs(ordered_envp, our_envp, env_count);
	
	// bubble sort
	sort_ascii(env_count, ordered_envp);
	print_strs(ordered_envp);

	// delete sorted.
	delete_strs(&ordered_envp);
}

void	add_to_envp(char *str, char **our_envp)
{
	size_t	strs_count;
	char	**new_envp;

	strs_count = get_strs_count(our_envp);
	new_envp = ft_calloc(strs_count + 2, sizeof(*new_envp));
	copy_strs(new_envp, our_envp, strs_count);
	new_envp[strs_count] = str;
	new_envp[strs_count + 1] = NULL;
}

void	exec_export(char **arglist, char **our_envp)
{
	// if (arglist is 'export' with no argument)
	// arglist[0] == export
	char	**str_quote_split;
	char	*str_quote_removed;
	size_t	str_len;

	if (arglist[1] != NULL) // 인자가 없는 경우.
		print_in_order(our_envp);
	else
	{
		str_quote_split = NULL;
		str_quote_removed = NULL;

		/** (1) remove quote */
		str_len = ft_strlen(arglist[1]);
		if (arglist[1][str_len - 1] == '\"')
			str_quote_split = ft_split(arglist[1], '\"');
		else if (arglist[1][str_len - 1] == '\'')
			str_quote_split = ft_split(arglist[1], '\'');

		if (str_quote_split != NULL)
		{
			str_quote_removed = ft_strjoin(str_quote_split[0], str_quote_split[1]);
			delete_strs(&str_quote_split);
		}
		else
			str_quote_removed = ft_strdup(arglist[1]);

		/** (2) check string and add to envp */
		if (ft_isdigit(str_quote_removed[0]))
		{
			free(str_quote_removed);
			ft_putstr_fd("export: ", STDERR_FILENO);
			ft_putstr_fd(arglist[1], STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
		}
		else
			add_to_envp(str_quote_removed, our_envp);
	}

	/** arglist[1] would be 
	 *  (1) HELLO="welcome"
	 *  (2) HELLO=welcome
	 *
	 *  --> remove quote
	 * */

}

/** 정렬 없이 출력 진행. 그렇다고 삽입한 대로 출력되는건 아닌듯... */
void	exec_env(char **arglist, char **our_envp)
{
	if (arglist[1] != NULL && arglist[1][0] == '-')
	{
		ft_putstr_fd("env: illegal option or argument", STDERR_FILENO);
		ft_putstr_fd(arglist[1], STDERR_FILENO);
	}
	else
		print_strs(our_envp);
}

/** 환경 변수 리스트에서 제거 */
void	exec_unset(char **arglist, char **our_envp)
{
	char	**str_split;
	char	**new_envp;
	size_t	strs_count;
	size_t	i;
	size_t	j;

	if (arglist[1] != NULL && arglist[1][0] == '-')
	{
		ft_putstr_fd("env: illegal option ", STDERR_FILENO);
		ft_putstr_fd(arglist[1], STDERR_FILENO);
	}
	else if (arglist[1] != NULL)
	{
		// copy to new_envp
		i = 0;
		j = i;
		strs_count = get_strs_count(our_envp);
		new_envp = ft_calloc(strs_count + 1, sizeof(*new_envp));
		while (i < strs_count)
		{
			str_split = ft_split(our_envp[i], '=');
			if (str_split != NULL \
					&& ft_strncmp(str_split[0], arglist[1], ft_strlen(arglist[1]) + 1) != 0)
			{
				// if not target, copy to new_envp
				new_envp[j] = our_envp[i];
				j++;
			}
			delete_strs(&str_split);
			i++;
		}
	}
}
