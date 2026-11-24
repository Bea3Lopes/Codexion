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
int	can_grant_request(t_simulation *simulation, t_coder *coder)
{
	long	current_time;
	
	if (simulation->stop_flag == 1)
		return (0);
	if (coder->left_dongle->available == 0)
		return (0);
	if (coder->right_dongle->available == 0)
		return (0);
	current_time = get_time_ms();
	if (coder->left_dongle->cooldown_until > current_time)
		return (0);
	if (coder->right_dongle->cooldown_until > current_time)
		return (0);
	return (1);
}

void	grant_request(t_coder *coder)
{
	coder->left_dongle->available = 0;
	log_state_locked(coder, "has taken a dongle");
	coder->right_dongle->available = 0;
	log_state_locked(coder, "has taken a dongle");
	coder->grant = 1;
}

int	find_best_candidate(t_simulation *sim)
{
	int		i;
	int		best;

	i = 0;
	best = -1;
	while (i < sim->heap->size)
	{
		if (can_grant_request(sim, sim->heap->coders_queue[i]))
		{
			if (best == -1
				|| has_priority(sim->heap->coders_queue[i],
								sim->heap->coders_queue[best]))
				best = i;
		}
		i++;
	}
	return (best);
}

void	dispatch_request(t_simulation *sim)
{
	t_coder	*coder_dispatched;
	int		best;

	pthread_mutex_lock(&sim->resource_mutex);
	best = find_best_candidate(sim);
	while (best != -1)
	{
		best = find_best_candidate(sim);
		coder_dispatched = sim->heap->coders_queue[best];
		remove_coder(sim->heap, best);
		grant_request(coder_dispatched);
		pthread_cond_broadcast(&sim->resource_cond);
		best = find_best_candidate(sim);
	}
	pthread_mutex_unlock(&sim->resource_mutex);
}

void	release_dongles(t_coder *coder)
{
	long	cooldown;
	t_simulation	*sim;
	
	sim = coder->sim;
	pthread_mutex_lock(&sim->resource_mutex);
	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	cooldown = coder->sim->config->dongle_cooldown + get_time_ms();
	coder->left_dongle->cooldown_until = cooldown;
	coder->right_dongle->cooldown_until = cooldown;
	pthread_mutex_unlock(&sim->resource_mutex);
}
