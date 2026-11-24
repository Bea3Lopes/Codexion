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
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_dongle
{
	int	id;
	int	available; // 0 -> not available, 1-> available
	long	cooldown_until;

}	t_dongle;

typedef enum e_scheduler_type
{
	EDF, //0
	FIFO //1

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

typedef struct s_simulation t_simulation;

typedef struct s_coder
{
	pthread_t	thread;
	int	id;
	int	compile_count;
	long	last_compile_start;
	long	deadline;
	long	time_request;
	int	grant;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_simulation	*sim;

}	t_coder;

typedef struct s_heap
{
	t_coder	**coders_queue;
	int	size;
	int	capacity;
}	t_heap;

typedef struct s_simulation
{
	t_config	*config;
	t_dongle	**dongles;
	t_coder	**coders;
	t_scheduler_type	scheduler;
	t_heap	*heap;
	long	start_time;
	int	stop_flag;
	int	completed_coders;
	pthread_t	monitor_thread; // monitor thread for main to join
	pthread_mutex_t	resource_mutex; // for stopflag and completed_coders
	pthread_mutex_t	write_mutex; //printing
	pthread_cond_t	resource_cond;
}	t_simulation;



int		main(int argc, char **argv);
//parse & init
int	ft_isdigit(char n);
int	initialize_config(t_config *config, char **argv);
int	parser(int argc, char **argv, t_config *config);
int	init_simulation(t_config *config, t_simulation *simulation);
t_dongle	**init_dongles(t_config *config);
t_coder	**init_coders(t_config *config, t_dongle **dongles_list);
//sim
int	thread_coders(t_simulation *sim, int number_coders);
int	run_simulation(t_simulation *sim);
void	*single_routine(void *arg);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
void	stop_simulation(t_simulation *sim);
void	sim_cleanup(t_simulation *sim);
//actions
void	do_compile(t_coder *coder);
void	do_debug(t_coder *coder);
void	do_refactor(t_coder *coder);
void	log_state(t_coder *coder, char *message);
void	log_state_locked(t_coder *coder, char *message);
//heap_functions
void		free_heap(t_heap *heap);
t_heap		*create_heap(int capacity);
int		has_priority(t_coder *a, t_coder *b);
void		swap(t_coder **a, t_coder **b);
void		insert_heap(t_simulation *sim, t_heap *heap, t_coder *coder);
void		heapify(t_heap *heap, int i);
t_coder		*extract_root(t_heap *heap);
void	remove_coder(t_heap *heap, int i);
//scheduler
int	can_grant_request(t_simulation *simulation, t_coder *coder);
void	grant_request(t_coder *coder);
int	find_best_candidate(t_simulation *sim);
void	dispatch_request(t_simulation *sim);
void	release_dongles(t_coder *coder);
//time
long	get_time_ms(void);
void	ft_usleep(long time_in_ms);
#endif
