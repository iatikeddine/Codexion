/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:01:07 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:01:08 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(char *str, t_coder *coder)
{
	long	timestamp;

	pthread_mutex_lock(&coder->args->print_lock);
	pthread_mutex_lock(&coder->args->state_lock);
	if (coder->args->ending_point == 't')
	{
		pthread_mutex_unlock(&coder->args->state_lock);
		pthread_mutex_unlock(&coder->args->print_lock);
		return ;
	}
	if (strcmp(str, "burned out") == 0)
		coder->args->ending_point = 't';
	pthread_mutex_unlock(&coder->args->state_lock);
	timestamp = get_current_time() - coder->args->starting_time;
	printf("%ld %d %s\n", timestamp, coder->coder_id, str);
	pthread_mutex_unlock(&coder->args->print_lock);
}

long	get_current_time(void)
{
	long			current_time;
	struct timeval	timevalue;

	gettimeofday(&timevalue, NULL);
	current_time = (timevalue.tv_sec * 1000) + (timevalue.tv_usec / 1000);
	return (current_time);
}

void	destroy_mutexes(t_arguments *args)
{
	int	i;

	pthread_mutex_destroy(&args->print_lock);
	pthread_mutex_destroy(&args->state_lock);
	i = 0;
	while (i < args->number_of_coders)
	{
		pthread_mutex_destroy(&args->dongles[i].lock);
		pthread_cond_destroy(&args->dongles[i].wait_cond);
		i++;
	}
}

int	threads_init(t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		args->coders[i].last_compile_time = args->starting_time;
		if (pthread_create(&args->coders[i].coder_thread, NULL, routine,
				&args->coders[i]) != 0)
		{
			fprintf(stderr, "Error: failed to create coder thread\n");
			pthread_mutex_lock(&args->state_lock);
			args->ending_point = 't';
			pthread_mutex_unlock(&args->state_lock);
			while (--i >= 0)
				pthread_join(args->coders[i].coder_thread, NULL);
			return (0);
		}
		i++;
	}
	return (1);
}

int	join_threads(t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		if (pthread_join(args->coders[i].coder_thread, NULL) != 0)
		{
			fprintf(stderr, "Error: failed to join thread\n");
			return (0);
		}
		i++;
	}
	return (1);
}
