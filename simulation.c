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

int	thread_coders(t_simulation *sim, int number_coders)
{
	int		i;
	int		thread_flag;
	t_coder	*coder;

	coder = sim->coders;
	i = 0;
	while (i < number_coders)
	{
		coder[i]->last_compile_start = sim->start_time;
		if (number_coders == 1)
			thread_flag = pthread_create(&coder[i]->thread, NULL, single_routine, coder[i]);
		else
			thread_flag = pthread_create(&coder[i]->thread, NULL, coder_routine, coder[i]);
		if (thread_flag != 0)
			return (i);
		i++;
	}
	return (i);
}

int	start_simulation(t_simulation *sim)
{
	int	i;
	int	monitor;
	int	nbr_thread_coders;
	int	flag;

	sim->start_time = get_time_ms();
	flag = pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim);
	if (!flag)
	{
		stop_simulation(sim);
		return (1);
	}
	nbr_thread_coders = thread_coders(sim, sim->config->number_coders);
	if (nbr_thread_coders != sim->config->number_coders)
	{
		flag = 1;
		stop_simulation(sim);
	}
	i = 0;
	while (i < nbr_thread_coders)
	{
		pthread_join(sim->coders[i]->thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
	return (flag);
}

void	*single_routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;
	long			dongle_cooldown;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (!sim->stop_flag)
	{
		do_compile(coder);
		printf("%l 1 is compiling", sim->);
		dongle_cooldown = get_time_ms() + sim->config->dongle_cooldown;
		do_debug(coder);
		printf("%l 1 is debugging, sim->")
		do_refactor(coder);
		printf("%l 1 is refactoring", sim->)
		ft_usleep(get_time_ms() - dongle_cooldown);
	}
	return (NULL);
}

void	*coder_routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (1)
	{
		if (sim->stop_flag|| coder->compile_count >= sim->config->number_compiles_required)
    		{
        		pthread_mutex_unlock(&sim->resource_mutex);
        		break;
    		}

    		pthread_mutex_unlock(&sim->resource_mutex);
		pthread_mutex_lock(&sim->resource_mutex);
		coder->grant = 0;
		insert_heap(sim, sim->heap, coder);
		pthread_mutex_unlock(&sim->resource_mutex);
		dispatch_request(sim);
		pthread_mutex_lock(&sim->resource_mutex);
		while (!coder->grant && !sim->stop_flag)
			pthread_cond_wait(&sim->resource_cond, &sim->resource_mutex);
		if (sim->stop_flag)
			break ;
		pthread_mutex_unlock(&sim->resource_mutex);
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
	int				nbr_compiles_required;
	long			time_to_burnout;
	int				i;

	sim = (t_simulation *)arg;
	nbr_compiles_required = sim->config->number_compiles_required;
	time_to_burnout = sim->config->time_to_burnout;
	while (sim->completed_coders < sim->config->number_coders)
	{
		if (sim->stop_flag)
			break ;
		i = 0;
		while (i < sim->config->number_coders)
		{
			if (sim->coders[i]->last_compile_start - get_time_ms() >= time_to_burnout)
			{
				stop_simulation(sim);
				return (NULL);
			}
			i++;
		}
		dispatch_request(sim);
		ft_usleep(500);
	}
	stop_simulation(sim);
	return (NULL);
}

void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->resource_mutex);
	sim->stop_flag = 1;
	pthread_cond_broadcast(&sim->resource_cond);
	pthread_mutex_unlock(&sim->resource_mutex);
}
