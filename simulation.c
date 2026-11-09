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

	i = 0;

	while (i < number_coders)
	{
		sim->coders[i]->last_compile_start = sim->start_time;
		if (number_coders == 1)
			thread_flag = pthread_create(&sim->coders[i]->thread, NULL, single_routine, &sim->coders[i]);
		else
			thread_flag = pthread_create(&sim->coders[i]->thread, NULL, coder_routine, &sim->coders[i]);
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

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	//start
	////request_dongles
	///wai for grant
	///compile
	///release dongles
	///debug
	///refactor
	///repeat until stop condition
}

void	*monitor_routine(void *arg)
{

}
