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

t_heap	*create_heap(int capacity)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return NULL;

	heap->size = 0;
	heap->capacity = capacity;
	heap->array = malloc(sizeof(int) * capacity);

	return heap;
}

void	swap(int *a. int *b)
{
	int temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void insert_heap(t_heap *heap, int value)
{
	int i;

	if (heap-> size >= heap->capacity)
	{
		printf("Heap overflow\n");
		return;
	}
	heap->size++;
	i = heap->size - 1;
	heap->array[i] = value;
	while (i != 0 && heap->array[(i - 1) / 2] < heap->array[i])
	{
		swap(&heap->array[i], &heap->array[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void heapify(t_heap *heap, int i)
{
	int largest;
	int left;
	int right;

	largest = i;
	left = 2 * i + 1;
	right = 2 * i + 2;

	if (left < heap->size && heap->array[left] > heap->array[largest])
		largest = left;
	if (right < heap->size && heap->array[right] > heap->array[largest])
		largest = right;

	if (largest != i)
	{
		swap(&heap->array[i], &heap->array[largest]);
		heapify(heap, largest);
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
	heapify(heap, 0);

	return root;
}
