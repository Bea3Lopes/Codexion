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

int init_simulation(t_config *config, t_simulation *simulation)
{
	simulation->config = config;
	simulation->dongles = init_dongles(config);
	if (!simulation->dongles)
		return 0;
	simulation->coders = init_coders(config, simulation->dongles);
	if(!simulation->coders)
		return 0;
	simulation->scheduler = config->scheduler;
	simulation->heap = create_heap(config->number_coders);
	if (!heap)
		return 0;
	simulation->start_time = 0;
	simulation->stop_flag = 0;
	simulation->completed_coders = 0;

	return 1;
}

t_dongle	*init_dongles(t_config *config)
{
	t_dongle	*dongles_list;
	int nbr_dongles;
	int i;

	nbr_dongles = config->number_coders;

	dongles_list = malloc(sizeof(t_dongle) * nbr_dongles);
	if (!dongles_list)
		return NULL;
	
	i = 0;
	while (++i <= nbr_dongles)
	{
		dongles_list[i - 1].id = i;
		dongles_list[i - 1].available = 1;
		dongles_list[i - 1].cooldown_until = 0;
	}

	return dongles_list;
}

t_coder	*init_coders(t_config *config, t_dongle *dongles_list)
{
	int i;
	t_coder	*coders_list;
	int	nbr_coders;

	nbr_coders = config->number_coders;
	coders_list = malloc(sizeof(t_coder) * nbr_coders);
	if (!coders_list)
		return NULL;

	i = 0;
	while (i <= nbr_coders)
	{
		coders_list[i].id = i + 1;
		coders_list[i].compile_count = 0;
		coders_list[i].last_compile_start = 0;
		coders_list[i].deadline = 0;
		coders_list[i].left_dongle = dongles_list[i];
		if (i == 0)
			coders_list[i].right_dongle = dongles_list[nbr_coders];
		else
			coders_list[i].right_dongle = dongles_list[i - 1];
		i++;
	}

	return coders_list;
}
