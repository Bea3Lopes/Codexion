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
int	grant_request(t_simulation *simulation, t_coder	*coder)
{
	if (simulation->stop_flag == 1)
		return 0;

	if (coder.left_dongle->available ==  0)
		return 0;

	if (coder.right_dongle->available ==  0)
		return 0;

	if (coder.left_dongle->cooldown_until > 0)
		return 0;

	if (coder.left_dongle->cooldown_until > 0)
		return 0;
	
	return 1;
}

int	request_dongles(t_coder *coder, t_simulation *sim)
//adds coder to queue
{
	insert_heap(sim->heap, coder);
	//heapify with new coder according to fifo or edf
}

void	dispatch_requests(t_simulation *simulation)
//checks queue and grants dongles to good requests according to fifo or edf
{
	if (grant_request(simulation, simulation->heap[0]) == 1)
	{
		grant_dongles()
	}
}

void	release_dongles(t_coder *coder)
{

}

int	request_has_priority
