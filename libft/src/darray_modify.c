/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   darray_modify.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 16:27:38 by minkyeki          #+#    #+#             */
/*   Updated: 2022/05/01 14:15:46 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "darray.h"

static void	ft_memcpy(void *dest, const void *src, size_t nbyte)
{
	size_t				i;
	unsigned char		*u_dest;
	const unsigned char	*u_src;

	u_dest = dest;
	u_src = src;
	i = 0;
	while (i < nbyte)
	{
		u_dest[i] = u_src[i];
		++i;
	}
}

void	*darray_reserve(t_darray *arr, size_t new_capacity)
{
	void	*new_data;

	new_data = NULL;
	if (arr->capacity < new_capacity)
	{
		new_data = new_data_malloc(new_capacity);
		if (new_data != NULL)
		{
			ft_memcpy(new_data, arr->data, (arr->size * sizeof(size_t)));
			free(arr->data);
			arr->data = new_data;
			arr->capacity = new_capacity;
		}
	}
	return ((void *)(new_data));
}

void	*darray_shrink_to_fit(t_darray *arr)
{
	void	*shrinked_data;

	shrinked_data = NULL;
	if (arr->size < arr->capacity)
	{
		shrinked_data = new_data_malloc(arr->size);
		if (shrinked_data != NULL)
		{
			ft_memcpy(shrinked_data, arr->data, (arr->size * sizeof(size_t)));
			free(arr->data);
			arr->data = shrinked_data;
			arr->capacity = arr->size;
		}
	}
	return (shrinked_data);
}

void	darray_push_back(t_darray *arr, void *data_in)
{
	void	*status;

	if (arr == NULL || data_in == NULL)
		return ;
	if (arr->size >= arr->capacity)
	{
		status = darray_reserve(arr, arr->capacity * 2);
		if (status == NULL)
			return ;
	}
	darray_set_data(arr, arr->size, data_in);
	(arr->size)++;
}

void	darray_pop_back(t_darray *arr)
{
	if (arr->size > 0)
	{
		darray_set_data(arr, arr->size - 1, NULL);
		(arr->size)--;
	}
}
