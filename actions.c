/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 12:21:57 by besoares          #+#    #+#             */
/*   Updated: 2026/11/12 12:22:54 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	do_compile(t_coder *coder)
{
	t_simulation	*sim;
	long			time_compile;

	sim = coder->sim;
	time_compile = coder->sim->config->time_to_compile;
	pthread_mutex_lock(&sim->resource_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&sim->resource_mutex);
	log_state(coder, "is compiling");
	ft_usleep(time_compile);
	pthread_mutex_lock(&sim->resource_mutex);
	coder->compile_count++;
	if (coder->compile_count == coder->sim->config->number_compiles_required)
		coder->sim->completed_coders++;
	pthread_mutex_unlock(&sim->resource_mutex);
	return ;
}

void	do_debug(t_coder *coder)
{
	long	time_debug;

	time_debug = coder->sim->config->time_to_debug;
	log_state(coder, "is debugging");
	ft_usleep(time_debug);
	return ;
}

void	do_refactor(t_coder *coder)
{
	long	time_refactor;

	time_refactor = coder->sim->config->time_to_refactor;
	log_state(coder, "is refactoring");
	ft_usleep(time_refactor);
	return ;
}

void	log_state(t_coder *coder, char *message)
{
	pthread_mutex_lock(&coder->sim->resource_mutex);
	log_state_locked(coder, message);
	pthread_mutex_unlock(&coder->sim->resource_mutex);
}

void	log_state_locked(t_coder *coder, char *message)
{
	if (coder->sim->stop_flag)
		return ;
	pthread_mutex_lock(&coder->sim->write_mutex);
	printf("%ld %d %s\n",
		get_time_ms() - coder->sim->start_time,
		coder->id,
		message);
	pthread_mutex_unlock(&coder->sim->write_mutex);
}
