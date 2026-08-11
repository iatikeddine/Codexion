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
	pthread_mutex_destroy(&args->lock_sim);
	pthread_cond_destroy(&args->wake_cond);
	i = 0;
	while (i < args->number_of_coders)
	{
		pthread_mutex_destroy(&args->dongles[i].lock);
		i++;
	}
}

void	free_all(t_arguments *args)
{
	if (args->coders)
		free(args->coders);
	if (args->dongles)
		free(args->dongles);
}

void	smart_sleep(long time_to_sleep, t_coder *coder)
{
	long	sleep_deadline;

	sleep_deadline = get_current_time() + time_to_sleep;
	while (get_current_time() < sleep_deadline)
	{
		if (check_simulation(coder) == 1)
			return ;
		usleep(500);
	}
}
