/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 13:08:13 by besoares          #+#    #+#             */
/*   Updated: 2026/11/25 13:09:24 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->resource_mutex);
	sim->stop_flag = 1;
	pthread_cond_broadcast(&sim->resource_cond);
	pthread_mutex_unlock(&sim->resource_mutex);
}

void	sim_cleanup(t_simulation *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->config->number_coders)
	{
		free(sim->coders[i]);
		free(sim->dongles[i]);
		i++;
	}
	free(sim->coders);
	free(sim->dongles);
	free_heap(sim->heap);
	pthread_mutex_destroy(&sim->resource_mutex);
	pthread_mutex_destroy(&sim->write_mutex);
	pthread_cond_destroy(&sim->resource_cond);
}

int	thread_coders(t_simulation *sim, int number_coders)
{
	int		i;
	int		thread_flag;
	t_coder	**coder;

	coder = sim->coders;
	i = 0;
	while (i < number_coders)
	{
		coder[i]->last_compile_start = sim->start_time;
		if (number_coders == 1)
			thread_flag = pthread_create(&coder[i]->thread,
					NULL, single_routine, coder[i]);
		else
			thread_flag = pthread_create(&coder[i]->thread,
					NULL, coder_routine, coder[i]);
		if (thread_flag != 0)
			return (i);
		i++;
	}
	return (i);
}

t_coder	*check_burnout(t_simulation *sim, long time_to_burnout)
{
	int	i;

	i = 0;
	while (i < sim->config->number_coders)
	{
		if (sim->coders[i]->compile_count
			< sim->config->number_compiles_required
			&& get_time_ms() - sim->coders[i]->last_compile_start
			> time_to_burnout)
			return (sim->coders[i]);
		i++;
	}
	return (NULL);
}
