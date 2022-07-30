/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:26:55 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/30 18:46:44 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environ.h"
#include "../main/helper.h"

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
	copy_strs_for_export(ordered_envp, our_envp, env_count);
	sort_ascii(env_count, ordered_envp);
	print_strs(ordered_envp);
	delete_strs(&ordered_envp);
}
