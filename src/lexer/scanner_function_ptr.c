/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_function_ptr.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 17:55:44 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/27 15:16:53 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scanner.h"

int	scanner_has_next(t_scanner *scan)
{
	return (scan->iter.f_has_next(&(scan->iter)));
}

char	scanner_next(t_scanner *scan)
{
	return (scan->iter.f_next(&(scan->iter)));
}

void	scanner_unget(t_scanner *scan)
{
	scan->iter.f_unget(&(scan->iter));
}

char	scanner_peek(t_scanner *scan)
{
	return (scan->iter.f_peek(&(scan->iter)));
}

void	scanner_skip_white_space(t_scanner *scan)
{
	scan->iter.f_skip_white_space(&(scan->iter));
}
