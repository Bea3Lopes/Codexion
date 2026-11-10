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
	int	i;
	int	thread_flag;
	t_coder	*coder;

	coder= sim->coders;
	i = 0;

	while (i < number_coders)
	{
		coder[i]->last_compile_start = sim->start_time;
		if (number_coders == 1)
			thread_flag = pthread_create(coder[i]->thread, NULL, single_routine, coder[i]);
		else
			thread_flag = pthread_create(coder[i]->thread, NULL, coder_routine, coder[i]);
		if (thread_flag != 0)
			return i;
		i++;
	}
	return i;
}
int	start_simulation(t_simulation *sim)
{
	int	i;
	int	monitor;
	int nbr_thread_coders;
	int	flag;

	sim->start_time = get_time_ms();
	nbr_thread_coders = thread_coders(sim, sim->config->number_coders);
	if (nbr_thread_coders != sim->config->number_coders &&
		pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
	{
		flag = 1;
		stop_simulation(sim);
	}
	i = 0;
	while (i < nbr_thread_coders)
		pthread_join(sim->coders[i]->thread, NULL);
	if (!flag)
		pthread_join(sim->monitor_thread, NULL);
	return flag;
}
void	*single_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	

}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int	res;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	//start
	while(!sim->stop_flag)
	{
	////register request, needs to have a mutex (figure later, maybe i will need request_dongles)
		insert_heap(sim, sim->heap, coder);
		//can do a dispatch_requests
		pthread_mutex_lock(&sim->resource_mutex);
		while (!coder->granted && !sim->stop_flag)
		 //instead do the pthread_cond_wait -> puts thread to sleep, so isnt constinoustly checking flag
			pthread_cond_wait(&sim->resource_cond, &sim->resource_mutex);
		pthread_mutex_unlock(&sim->resource_mutex);
		if (sim->stop_flag)
			break;
	///compile
		do_compile(coder);
	///release dongles
		release_dongles(coder);
	///checks the heap and decides who gets dongles
		dispatch_requests(); //set coder->grant = 1 and signals the cond var
	///debug
		do_debug(coder);
	///refactor
		do_refactor(coder);
	///repeat until stop condition
	}
}

void	*monitor_routine(void *arg)
{

}
