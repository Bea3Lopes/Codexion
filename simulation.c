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
	t_coder	**coder;

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
	i = 0;
	while (i < nbr_thread_coders)
	{
		pthread_join(sim->coders[i]->thread, NULL);
		i++;
	}
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
	while (!sim->stop_flag)
		ft_usleep(sim->config->time_to_burnout);
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
		pthread_mutex_lock(&sim->resource_mutex);
		if (sim->stop_flag || coder->compile_count >= sim->config->number_compiles_required)
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
		{
			pthread_mutex_unlock(&sim->resource_mutex);
			break ;
		}
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
	long			time_to_burnout;
	int				i;

	sim = (t_simulation *)arg;
	time_to_burnout = sim->config->time_to_burnout;
	while (1)
	{
		pthread_mutex_lock(&sim->resource_mutex);
		if (sim->stop_flag)
		{
			pthread_mutex_unlock(&sim->resource_mutex);
			break ;
		}
		if (sim->completed_coders >= sim->config->number_coders)
		{
			pthread_mutex_unlock(&sim->resource_mutex);
			stop_simulation(sim);
			break ;
		}
		i = 0;
		while (i < sim->config->number_coders)
		{
			while (sim->coders[i]->compile_count
				>= sim->config->number_compiles_required)
				i++;
			if (get_time_ms() - sim->coders[i]->last_compile_start >= time_to_burnout)
			{
				pthread_mutex_unlock(&sim->resource_mutex);
				log_state(sim->coders[i], "burned out");
				stop_simulation(sim);
				return (NULL);
			}
			i++;
		}
		pthread_mutex_unlock(&sim->resource_mutex);
		dispatch_request(sim);
		ft_usleep(100);
	}
	return (NULL);
}

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
