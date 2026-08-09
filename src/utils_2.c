/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:01:02 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:01:03 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all(t_arguments *args)
{
	if (args->coders)
		free(args->coders);
	if (args->dongles)
		free(args->dongles);
}

void	change_status(t_coder *coder)
{
	print_status("is compiling", coder);
	pthread_mutex_lock(&coder->args->state_lock);
	coder->last_compile_time = get_current_time();
	coder->number_of_compiles++;
	pthread_mutex_unlock(&coder->args->state_lock);
	smart_sleep(coder->args->time_to_compile, coder);
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

int	check_simulation(t_coder *coder)
{
	pthread_mutex_lock(&coder->args->state_lock);
	if (coder->args->ending_point == 't')
	{
		pthread_mutex_unlock(&coder->args->state_lock);
		return (1);
	}
	pthread_mutex_unlock(&coder->args->state_lock);
	return (0);
}