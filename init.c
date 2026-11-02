/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bea3 <besoares@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 15:12:09 by bea3              #+#    #+#             */
/*   Updated: 2026/09/22 16:14:06 by bea3             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_simulation(t_config config, t_simulation simulation)
{
	simulation->dongles = init_dongles(config); //or NULL
	simulation->config = config;
	simulation->coders = init_coders(config); //or NULL
	simulation->scheduler = init_scheduler();
	simulation->start_time = 0;
	simulation->stop_flag = 0;
	simulation->completed_coders = 0;
	pthread_init(&simulation->monitor_thread, NULL);
	pthread_mutex_init(&simulation->stop_mutex, NULL);
	pthread_mutex_init(&simulation->write_mutex, NULL);



	return 1;
}

t_coders	init_coders(t_config config)
{
	int i;
	t_coder	*first_coder;
	t_coder	current_coder;

	while (i < config->number_of_coders)
	{
		pthread_init(&)
		if (i == 0)
			first_coder = &current_coder;
	}
}
