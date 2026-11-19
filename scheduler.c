/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 15:44:15 by besoares          #+#    #+#             */
/*   Updated: 2026/11/03 18:09:29 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
//0 -> not request, 1-> can grant request
int	can_grant_request(t_simulation *simulation, t_coder	*coder)
{
	if (simulation->stop_flag == 1)
		return (0);
	if (coder->left_dongle->available == 0)
		return (0);
	if (coder->right_dongle->available == 0)
		return (0);
	if (coder->left_dongle->cooldown_until > get_time_ms())
		return (0);
	if (coder->right_dongle->cooldown_until > get_time_ms())
		return (0);
	return (1);
}

void	grant_request(t_coder *coder)
{
	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	coder->grant = 1;
}

void	dispatch_request(t_simulation *sim)
{
	t_coder	*coder_dispatched;
	t_heap	*heap;
	int		i;

	heap = sim->heap;
	pthread_mutex_lock(&sim->resource_mutex);
	i = 0;
	while (i < heap->size)
	{
		if (can_grant_request(sim, heap->coders_queue[i]))
		{
			coder_dispatched = heap->coders_queue[i];
			remove_coder(heap, i);
			grant_request(coder_dispatched);
			pthread_cond_broadcast(&sim->resource_cond);
			i = -1;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->resource_mutex);
}

void	release_dongles(t_coder *coder)
{
	long	cooldown;

	pthread_mutex_lock(&sim->resource_mutex);
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	cooldown = coder->sim->config->dongle_cooldown + get_time_ms();
	coder->left_dongle->cooldown_until = cooldown;
	coder->right_dongle->cooldown_until = cooldown;
	pthread_mutex_unlock(&sim->resource_mutex);
}
