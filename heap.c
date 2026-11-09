/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:30:32 by besoares          #+#    #+#             */
/*   Updated: 2026/11/03 14:30:36 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//fifo - arrival time (menor) (deadline)
//edf - last_compile_start + time_to_burnout (deadline)

t_heap	*create_heap(int capacity)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return NULL;

	heap->size = 0;
	heap->capacity = capacity;
	heap->coders_queue = malloc(sizeof(t_coder *) * capacity);
	if (!heap->coders_queue)
		//maybe dar free a heap tbm
		return NULL;

	return heap;
}

int	has_priority(t_coder *a, t_coder *b)
{
	if (a->deadline < b->deadline)
		return 1;
	if (a->deadline == b->deadline && a->time_request < b->time_request)
		return 1;
	return 0;
}

void	swap(t_coder *a, t_coder *b)
{
	t_coder* temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void insert_heap(t_simulation *sim, t_heap *heap, t_coder *coder)
{
	int i;
	long	time;

	if (heap->size >= heap->capacity)
	{
		printf("Heap overflow\n");
		return;
	}
	heap->size++;
	i = heap->size - 1;
	heap->coders_queue[i] = coder;
	time = get_time_ms();
	if (sim->scheduler == EDF)
		coder->deadline = coder->last_compile_start + sim->config->time_to_burnout;
	else
		coder->deadline = time;
	
	coder->time_request = time;

	while (i != 0 && has_priority(heap->coders_queue[1], 
							   heap->coders_queue[(i - 1) / 2])
	{
		swap(&heap->coders_queue[i], &heap->coders_queue[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void heapify(t_heap *heap, int i)
{
	int smallest;
	int left;
	int right;

	smallest = i;
	left = 2 * i + 1;
	right = 2 * i + 2;

	if (left < heap->size && has_priority(heap->coders_queue[left], 
									   heap->coders_queue[smallest]))
		smallest = left;
	if (right < heap->size && has_priority(heap->coders_queue[right], 
										heap->coders_queue[smallest]))
		smallest = right;

	if (smallest != i)
	{
		swap(&heap->coders_queue[i], &heap->coders_queue[smallest]);
		heapify(heap, smallest);
	}
}

t_coder	*extract_root(t_heap *heap)
{
	t_coder	*root;

	if (heap->size <= 0)
		return NULL;
	if (heap->size == 1)
	{
		heap->size--;
		return heap->coders_queue[0];
	}
	root = heap->coders_queue[0];
	heap->coders_queue[0] = heap->coders_queue[heap->size - 1];
	heap->size--;
	heapify(heap, 0);

	return root;
}
