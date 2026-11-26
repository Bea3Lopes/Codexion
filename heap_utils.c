/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 12:24:27 by besoares          #+#    #+#             */
/*   Updated: 2026/11/25 12:25:55 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	has_priority(t_coder *a, t_coder *b)
{
	if (a->deadline > b->deadline)
		return (1);
	if (a->deadline == b->deadline && a->time_request > b->time_request)
		return (1);
	return (0);
}

void	free_heap(t_heap *heap)
{
	if (!heap)
		return ;
	free(heap->coders_queue);
	free(heap);
	return ;
}

void	swap(t_coder **a, t_coder **b)
{
	t_coder	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
