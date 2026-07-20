/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:58 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:00:59 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"

static int	check_simulation(t_coder *coder)
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

static void	compiling(t_coder *coder)
{
	if (coder->args->number_of_coders == 1)
	{
		take_dongle(coder->right_dongle, coder);
		while (check_simulation(coder) == 0)
			usleep(1000);
		return ;
	}
	else
	{
		if (coder->coder_id % 2 == 0)
		{
			usleep(15000);
			take_dongle(coder->right_dongle, coder);
			take_dongle(coder->left_dongle, coder);
		}
		else
		{
			take_dongle(coder->left_dongle, coder);
			take_dongle(coder->right_dongle, coder);
		}
	}
	change_status(coder);
	drop_dongle(coder->left_dongle, coder->args->dongle_cooldown);
	drop_dongle(coder->right_dongle, coder->args->dongle_cooldown);
}

static void	debugging(t_coder *coder)
{
	print_status("is debugging", coder);
	usleep(coder->args->time_to_debug * 1000);
}

static void	refactoring(t_coder *coder)
{
	print_status("is refactoring", coder);
	usleep(coder->args->time_to_refactor * 1000);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (check_simulation(coder) == 0)
	{
		compiling(coder);
		if (check_simulation(coder) == 1)
			return (NULL);
		debugging(coder);
		if (check_simulation(coder) == 1)
			return (NULL);
		refactoring(coder);
	}
	return (NULL);
}
