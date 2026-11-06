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
	heap->coders_queue = malloc(sizeof(t_coder) * capacity);

	return heap;
}

void	swap(int *a. int *b)
{
	int temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void insert_heap(t_simulation *sim, t_heap *heap, t_coder *coder)
{
	int i;

	if (heap->size >= heap->capacity)
	{
		printf("Heap overflow\n");
		return;
	}
	heap->size++;
	i = heap->size - 1;
	heap->coders_queue[i] = *coder;
	if (sim->scheduler == "EDF")
		coder->deadline = coder->last_compile_start + sim->config->time_to_burnout;
	else
		coder->deadline = arrival_time;

	while (i != 0 && heap->coders_queue[(i - 1) / 2]->deadline < heap->coders_queue[i]->deadline)
	{
		swap(&heap->coders_queue[i], &heap->coders_queue[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void heapify_up(t_heap *heap, long i)
{
	int largest;
	int left;
	int right;

	largest = i;
	left = 2 * i + 1;
	right = 2 * i + 2;

	if (left < heap->size && heap->array[left]->deadline > heap->array[largest]->deadline)
		largest = left;
	if (right < heap->size && heap->array[right]->deadline > heap->array[largest]->deadline)
		largest = right;

	if (largest != i)
	{
		swap(&heap->array[i], &heap->array[largest]);
		heapify_up(heap, largest);
	}
}

int extract_max(t_heap *heap)
{
	int root;

	if (heap->size <= 0)
		return 0;
	if (heap->size == 1)
		return heap->array[0];
	
	root = heap->array[0];
	heap->array[0] = heap->array[heap->size - 1];
	heap->size--;
	heapify_up(heap, 0);

	return root;
}
