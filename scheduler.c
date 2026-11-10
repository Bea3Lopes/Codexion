/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 15:44:15 by besoares          #+#    #+#             */
/*   Updated: 2026/11/03 18:09:29 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
//0 -> not request, 1-> can grant request
int	can_grant_request(t_simulation *simulation, t_coder	*coder)
{
	if (simulation->stop_flag == 1)
		return 0;

	if (coder->left_dongle->available ==  0)
		return 0;

	if (coder->right_dongle->available ==  0)
		return 0;

	if (coder->left_dongle->cooldown_until > get_time_ms())
		return 0;

	if (coder->right_dongle->cooldown_until > get_time_ms())
		return 0;
	
	return 1;
}

int	request_dongles(t_coder *coder, t_simulation *sim)
//adds coder to queue
{
	insert_heap(sim, sim->heap, coder);
	if (sim->heap[0]->id == coder->id && can_grant_request(sim, coder))
		return 1;
	else
		return 0
}

void	dispatch_request(t_coder *coder)
//checks queue and grants dongles to good requests according to fifo or edf
{
	extract_root(coder->sim->heap)
	coder->left_dongle->available = 0;
	coder->right_dongle->available = 0;
	coder->last_compile_start = get_time_ms();
	coder->compile_count++;
	sleep(coder->sim->config->time_to_compile);


}

void	release_dongles(t_coder *coder)
{
	long	cooldown;

	coder->left_dongle->available = 1;
	coder->right_dongle->available = 1;
	cooldown = coder->sim->config->dongle_cooldown + get_time_ms();
	coder->left_dongle->cooldown_until = cooldown;
	coder->right_dongle->cooldown_until = cooldown;

	//another dispatch
}

