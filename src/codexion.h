/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:04 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:00:05 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;

typedef struct s_arguments
{
	long			number_of_coders;
	long			number_of_compiles_required;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			dongle_cooldown;
	char			scheduler;

	long			starting_time;
	char			ending_point;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	state_lock;
	pthread_mutex_t	lock_sim;
	pthread_cond_t	wake_cond;

}	t_arguments;

typedef struct s_coder
{
	int				coder_id;
	long			number_of_compiles;
	long			last_compile_time;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_arguments		*args;
	pthread_t		coder_thread;
}	t_coder;

typedef struct s_request
{
	t_coder	*coder;
	long	arrival_time;
	long	time_left_to_burnout;
}	t_request;

typedef struct s_heap
{
	t_request	requests[2];
	int			current_size;
}	t_heap;

typedef struct s_dongle
{
	int				dongle_id;
	int				is_available;
	long			cooldown;
	t_heap			queue;
	pthread_mutex_t	lock;
}	t_dongle;

int		is_valid_number(const char *str);
int		str_trim(char **ptr);
int		fill_input_data(t_arguments *args, char **argv);
int		parse_args(int argc, char **argv);
int		sim_init(t_arguments *args);
int		threads_init(t_arguments *args);
int		join_threads(t_arguments *args);
int		check_simulation(t_coder *coder);
int		take_dongles(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder);
int		both_ready(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder);
long	get_current_time(void);
void	push_request(t_heap *queue, t_request req, char scheduler_type);
void	pop_request(t_heap *queue);
void	print_status(char *str, t_coder *coder);
void	*routine(void *arg);
void	*routine_monitoring(void *arg);
void	destroy_mutexes(t_arguments *args);
void	free_all(t_arguments *args);
void	change_status(t_coder *coder);
void	drop_dongles(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder);
void	fill_requests(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder);
void	one_coder(t_coder *coder);
void	smart_sleep(long time_to_sleep, t_coder *coder);

#endif