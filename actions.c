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
	long	time_compile;

	time_compile = coder->sim->config->time_to_compile;
	coder->last_compile_start = get_time_ms();
	ft_usleep(time_compile);
	coder->compile_count++;
	if (coder->compile_count == coder->sim->config->number_of_compiles_required)
		coder->sim->completed_coders++;
	return ;
}

void	do_debug(t_coder *coder)
{
	long	time_debug;

	time_debug = coder->sim->config->time_to_debug;
	ft_usleep(time_debug);
	return ;
}

void	do_refactor(t_coder *coder)
{
	long	time_refactor;

	time_refactor = coder->sim->config->time_to_refactor;
	ft_usleep(time_refactor);
	return ;
}
