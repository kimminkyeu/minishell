/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray_iterate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 16:46:20 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/10 18:19:21 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "darray.h"

void	darray_iterate(t_darray *arr, void (*f)(void *))
{
	size_t	i;

	i = 0;
	while (i < arr->size)
	{
		f(arr->data[i]);
		i++;
	}
}

t_darray	*darray_map_malloc(t_darray *arr, void *(*f)(void *))
{
	size_t		i;
	t_darray	*new_arr;

	new_arr = new_darray_malloc(arr->size);
	if (new_arr != NULL)
	{
		i = 0;
		while (i < arr->size)
		{
			darray_push_back(new_arr, f(arr->data[i]));
			i++;
		}
	}
	return (new_arr);
}

int	darray_is_empty(t_darray *arr)
{
	size_t	i;

	i = 0;
	while (i < arr->size)
	{
		if (arr->data[i] != NULL)
			return (0);
		++i;
	}
	return (1);
}

void	*darray_get_last(t_darray *arr)
{
	if (arr->size == 0)
		return (NULL);
	return (arr->data[arr->size - 1]);
}
