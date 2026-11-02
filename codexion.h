#ifndef CODEXION_H

# define CODEXION_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>
# include <pthread.h>

typedef struct s_coder
{
	pthread_t	thread;
	int	id;
	int	compile_count;
	long	last_compile_start;
	long	deadline;
	t_dongle	left_dongle;
	t_dongle	right_dongle;
	t_simulation	simulation;

}	t_coder;

typedef struct s_dongle
{
	int	id;
	char	*availability;
	long	cooldown_until;
	t_coder	owner;
}	t_dongle;

typedef struct	s_scheduler
{
	t_scheduler_type	type;

}

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
	t_scheduler	scheduler;
}	t_config;

typedef struct s_simulation
{
	t_config	*config;
	t_dongle	*dongles;
	t_coder	*coders;
	t_scheduler	scheduler;
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

#endif
