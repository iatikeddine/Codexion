/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:49 by iatikedd          #+#    #+#             */
/*   Updated: 2026/08/11 11:52:37 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_arguments *args)
{
	int		i;
	long	last_compile_time;
	t_coder	*coder;

	i = 0;
	while (i < args->number_of_coders)
	{
		coder = &args->coders[i];
		pthread_mutex_lock(&args->state_lock);
		last_compile_time = coder->last_compile_time;
		pthread_mutex_unlock(&args->state_lock);
		if (get_current_time() - last_compile_time >= args->time_to_burnout)
		{
			print_status("burned out", coder);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	check_required_compiles(t_arguments *args)
{
	int		i;
	int		count;
	t_coder	*coder;

	i = 0;
	count = 0;
	while (i < args->number_of_coders)
	{
		coder = &args->coders[i];
		pthread_mutex_lock(&args->state_lock);
		if (coder->number_of_compiles >= args->number_of_compiles_required)
			count++;
		pthread_mutex_unlock(&args->state_lock);
		i++;
	}
	if (count == args->number_of_coders)
	{
		pthread_mutex_lock(&args->state_lock);
		args->ending_point = 't';
		pthread_mutex_unlock(&args->state_lock);
		return (0);
	}
	return (1);
}

void	broadcast(t_arguments *args)
{
	pthread_mutex_lock(&args->lock_sim);
	pthread_cond_broadcast(&args->wake_cond);
	pthread_mutex_unlock(&args->lock_sim);
}

void	*routine_monitoring(void *arg)
{
	t_arguments	*args;

	args = (t_arguments *)arg;
	while (1)
	{
		if (check_burnout(args) == 0)
		{
			broadcast(args);
			return (NULL);
		}
		if (check_required_compiles(args) == 0)
		{
			broadcast(args);
			return (NULL);
		}
		usleep(500);
		broadcast(args);
	}
}
