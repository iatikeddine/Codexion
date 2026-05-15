/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:33:12 by iatikedd          #+#    #+#             */
/*   Updated: 2026/05/11 11:33:27 by iatikedd         ###   ########.fr       */
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

// Represents all data needed for simulation and accessed by all coders
typedef struct	s_arguments{
	int	number_of_coders;
	long	number_of_compiles_required;
	long	time_to_burnout;
	long	time_to_compile;
	long	time_to_debug;
	long	time_to_refactor;
	long	dongle_cooldown;
	char	scheduler;

	long	starting_time; //time
	char	ending_point; // bool t or f sim ended or no
	t_dongle	*dongles;
	t_coder	*coders;

}	t_arguments;

// Represents each individual coder infos
typedef struct s_coder
{
	int	coder_id;
	int	number_of_compiles;
	int	last_compile_time;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	pthread_t	coder_thread;
	t_arguments	*ags;
}	t_coder;

// Represents a single dongle simply a mutex with ID
typedef struct s_dongle
{
	int	dongle_id;
	pthread_mutex_t	dongle;
}	t_dongle;

int	is_valid_number(const char *str);
int	is_all_alpha(const char *str);
int	str_trim(char **ptr);
int fill_input_data(t_arguments *args, char **argv);
int	parse_args(int argc, char **argv, t_arguments *args);
void    to_lower(char *str);

#endif

