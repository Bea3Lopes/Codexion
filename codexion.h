/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besoares <besoares@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:31:19 by besoares          #+#    #+#             */
/*   Updated: 2026/11/03 14:31:21 by besoares         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

# define CODEXION_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>
# include <pthread.h>


typedef struct s_dongle
{
	int	id;
	int	available;
	long	cooldown_until;

}	t_dongle;

typedef enum e_scheduler_type
{
	EDF,
	FIFO

}	t_scheduler_type;

typedef struct s_config
{
	int	number_coders;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	int	number_compiles_required;
	long	dongle_cooldown;
	t_scheduler_type	scheduler;

}	t_config;

typedef struct s_coder
{
	pthread_t	thread;
	int	id;
	int	compile_count;
	long	last_compile_start;
	long	deadline;
	t_dongle	left_dongle;
	t_dongle	right_dongle;

}	t_coder;

typedef struct s_heap
{
	int	*array;
	int	size;
	int	capacity;
}	t_heap;

typedef struct s_simulation
{
	t_config	*config;
	t_dongle	*dongles;
	t_coder	*coders;
	t_scheduler_type	scheduler;
	long	start_time;
	int	stop_flag;
	int	completed_coders;
	pthread_t	monitor_thread; // monitor thread for main to join
	pthread_mutex_t	stop_mutex; // for stopflag and completed_coders
	pthread_mutex_t	write_mutex; //printing
}	t_simulation;



int	main(int argc, char **argv);
int	ft_isdigit(char n);
int	initialize_config(t_config *config, char **argv);
int	parser(int argc, char **argv, t_config *config);
t_dongle	*init_dongles(t_config *config);
t_coder	*init_coders(t_config *config, t_dongle *dongles_list);
int init_simulation(t_config *config, t_simulation *simulation);
t_heap	*create_heap(int capacity);
void	swap(int *a. int *b);
void insert_heap(t_heap *heap, int value);
void heapify(t_heap *heap, int i);
int extract_max(t_heap *heap);

#endif
