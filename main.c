/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:31:09 by besoares          #+#    #+#             */
/*   Updated: 2026/11/03 14:31:11 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// t_config -> stack (i know the size)
// t_simulation -> stack
// coders[N]-> heap (malloc)
// dongles[N]-> heap
int	main(int argc, char **argv)
{
	t_config config;
	t_simulation simulation;

	if (parser(argc, argv, &config) == 0)
		return (1);

	if (init_simulation(&config, &simulation) == 0)
		return (1);
	if (pthread_mutex_init(&simulation.resource_mutex, NULL) != 0)
    	{
        	printf("Resource mutex initialization failed\n");
        	sim_cleanup(&simulation);
        	return (1);
    	}

    	if (pthread_mutex_init(&simulation.write_mutex, NULL) != 0)
    	{
        	printf("Write mutex initialization failed\n");
        	pthread_mutex_destroy(&simulation.resource_mutex);
        	sim_cleanup(&simulation);
        return (1);
    	}

    	if (pthread_cond_init(&simulation.resource_cond, NULL) != 0)
    	{
        	printf("Condition variable initialization failed\n");
        	pthread_mutex_destroy(&simulation.resource_mutex);
        	pthread_mutex_destroy(&simulation.write_mutex);
        	sim_cleanup(&simulation);
        	return (1);
    	}

    	if (run_simulation(&simulation) != 0)
        	printf("Simulation encountered an error\n");

    	sim_cleanup(&simulation);

    	return (0);
}
