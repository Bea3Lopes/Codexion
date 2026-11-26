/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 16:05:35 by besoares          #+#    #+#             */
/*   Updated: 2026/11/09 18:03:23 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	run_simulation(t_simulation *sim)
{
	int	i;
	int	nbr_thread_coders;
	int	flag;

	flag = 0;
	sim->start_time = get_time_ms();
	nbr_thread_coders = thread_coders(sim, sim->config->number_coders);
	if (nbr_thread_coders != sim->config->number_coders)
	{
		flag = 1;
		stop_simulation(sim);
	}
	if (!flag)
	{
		flag = pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim);
		if (flag)
			stop_simulation(sim);
	}
	i = -1;
	while (++i < nbr_thread_coders)
		pthread_join(sim->coders[i]->thread, NULL);
	if (!flag)
		pthread_join(sim->monitor_thread, NULL);
	return (flag);
}

void	*single_routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	log_state(coder, "has taken a dongle");
	while (1)
	{
		pthread_mutex_lock(&sim->resource_mutex);
		if (sim->stop_flag)
		{
			pthread_mutex_unlock(&sim->resource_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->resource_mutex);
		ft_usleep(sim->config->time_to_burnout);
	}
	return (NULL);
}

static int	request_dongles(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->resource_mutex);
	coder->grant = 0;
	insert_heap(sim, sim->heap, coder);
	pthread_mutex_unlock(&sim->resource_mutex);
	dispatch_request(sim);
	pthread_mutex_lock(&sim->resource_mutex);
	while (!coder->grant && !sim->stop_flag)
		pthread_cond_wait(&sim->resource_cond, &sim->resource_mutex);
	if (sim->stop_flag)
	{
		pthread_mutex_unlock(&sim->resource_mutex);
		return (0);
	}
	pthread_mutex_unlock(&sim->resource_mutex);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (1)
	{
		pthread_mutex_lock(&sim->resource_mutex);
		if (sim->stop_flag || coder->compile_count
			>= sim->config->number_compiles_required)
		{
			pthread_mutex_unlock(&sim->resource_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->resource_mutex);
		if (!request_dongles(coder))
			break ;
		do_compile(coder);
		release_dongles(coder);
		dispatch_request(sim);
		do_debug(coder);
		do_refactor(coder);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	t_coder			*coder;

	sim = (t_simulation *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->resource_mutex);
		if (sim->stop_flag || sim->completed_coders
			>= sim->config->number_coders)
		{
			pthread_mutex_unlock(&sim->resource_mutex);
			break ;
		}
		coder = check_burnout(sim, sim->config->time_to_burnout);
		pthread_mutex_unlock(&sim->resource_mutex);
		if (coder)
		{
			log_state(coder, "burned out");
			break ;
		}
		dispatch_request(sim);
		ft_usleep(10);
	}
	stop_simulation(sim);
	return (NULL);
}
