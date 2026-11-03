/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:31:01 by besoares          #+#    #+#             */
/*   Updated: 2026/11/03 14:31:03 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

int	ft_isdigit(char n)
{
	if (n >= '0' && n <= '9')
		return 1;
	else
		return 0;

}

int initialize_config(t_config *config, char** argv)
{
	config->number_coders = atoi(argv[1]);
	if (config->number_coders <= 0)
		return 0;
	config->time_to_burnout = atoi(argv[2]);
	if (config->time_to_burnout <= 0)
		return 0;
	config->time_to_compile = atoi(argv[3]);
	if (config->time_to_compile <= 0)
		return 0;
	config->time_to_debug = atoi(argv[4]);
	if (config->time_to_debug <= 0)
		return 0;
	config->time_to_refactor = atoi(argv[5]);
	if (config->time_to_refactor <= 0)
		return 0;
	config->number_compiles_required = atoi(argv[6]);
	if (config->number_compiles_required <= 0)
		return 0;
	config->dongle_cooldown = atoi(argv[7]);
	if (config->dongle_cooldown <= 0)
		return 0;

	if (strcmp(argv[8], "edf") == 0)
		config->scheduler = EDF;
	else
		config->scheduler = FIFO;

	return 1;
}

int	parser(int argc, char **argv, t_config *config)
{
	int j;
	int i;

	if (argc != 9)
	{
		printf("ERROR: number of args should be 8");
		return 0;
	}
	j = 1;
	while (j < 8)
	{
		i = 0;
		while (argv[j][i])
		{
			if (ft_isdigit(argv[j][i]) == 0)
			{
				printf("ERROR: not ints");
				return 0;
			}
			i++;
		}
		j++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		{
			printf("ERROR: should be fifo or edf");
			return 0;
		}
	if (initialize_config(config, argv) == 0)
	{
		printf("ERROR: initialize_config");
		return 0;
	}
	return 1;
}


