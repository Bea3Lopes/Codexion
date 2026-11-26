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

int	ft_isdigit(char n)
{
	if (n >= '0' && n <= '9')
		return (1);
	else
		return (0);
}

static int	print_error(char *type, char *error)
{
	fprintf(stderr, "Invalid argument '%s': %s\n", type, error);
	return (0);
}

static long	ft_atol(char *arg)
{
	long	digit;
	long	n;

	n = 0;
	while (*arg)
	{
		digit = *arg - '0';
		if (n > LONG_MAX / 10
			|| (n == LONG_MAX / 10 && digit > LONG_MAX % 10))
			return (-1);
		n = n * 10 + digit;
		arg++;
	}
	return (n);
}

int	initialize_config(t_config *config, char **argv)
{
	config->number_coders = ft_atol(argv[1]);
	if (config->number_coders <= 0 || config->number_coders > INT_MAX)
		return (print_error("number_coders", argv[1]));
	config->number_coders = (int)config->number_coders;
	config->time_to_burnout = ft_atol(argv[2]);
	if (config->time_to_burnout <= 0)
		return (print_error("time_to_burnout", argv[2]));
	config->time_to_compile = ft_atol(argv[3]);
	if (config->time_to_compile <= 0)
		return (print_error("time_to_compile", argv[3]));
	config->time_to_debug = ft_atol(argv[4]);
	if (config->time_to_debug <= 0)
		return (print_error("time_to_debug", argv[4]));
	config->time_to_refactor = ft_atol(argv[5]);
	if (config->time_to_refactor <= 0)
		return (print_error("time_to_refactor", argv[5]));
	config->number_compiles_required = ft_atol(argv[6]);
	if (config->number_compiles_required <= 0
		|| config->number_compiles_required > INT_MAX)
		return (print_error("number_compiles_required", argv[6]));
	config->number_compiles_required = (int)config->number_compiles_required;
	config->dongle_cooldown = ft_atol(argv[7]);
	if (config->dongle_cooldown < 0)
		return (print_error("dongle_cooldown", argv[7]));
	return (1);
}

int	parser(int argc, char **argv, t_config *config)
{
	int	j;
	int	i;

	if (argc != 9)
		return (print_error("number of arguments", "should be 8"));
	j = 0;
	while (++j < 8)
	{
		i = -1;
		while (argv[j][++i])
		{
			if (ft_isdigit(argv[j][i]) == 0)
				return (print_error(argv[j], "argument is not an int"));
		}
	}
	if (strcmp(argv[8], "edf") == 0)
		config->scheduler = EDF;
	else if (strcmp(argv[8], "fifo") == 0)
		config->scheduler = FIFO;
	else
		return (print_error(argv[8], "should be 'edf' or 'fifo'"));
	return (initialize_config(config, argv));
}
