/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 08:29:03 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/13 09:05:19 by iatikedd         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/codexion.h"

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

void	threads_init(t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		args->coders[i].last_compile_time = get_current_time();
		pthread_create(&args->coders[i].coder_thread, NULL, routine,
			&args->coders[i]);
		i++;
	}
}

void	join_threads(t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		pthread_join(args->coders[i].coder_thread, NULL);
		i++;
	}
}
