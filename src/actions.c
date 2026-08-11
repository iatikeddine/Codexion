/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:35 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:00:36 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	reserve_dongles(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder)
{
	dongle_1->is_available = 0;
	dongle_2->is_available = 0;
	pop_request(&dongle_1->queue);
	pop_request(&dongle_2->queue);
	print_status("has taken a dongle", coder);
	print_status("has taken a dongle", coder);
	pthread_mutex_unlock(&dongle_2->lock);
	pthread_mutex_unlock(&dongle_1->lock);
	pthread_mutex_unlock(&coder->args->lock_sim);
}

int	take_dongles(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder)
{
	pthread_mutex_lock(&coder->args->lock_sim);
	if (coder->args->number_of_coders == 1)
	{
		one_coder(coder);
		pthread_mutex_unlock(&coder->args->lock_sim);
		return (0);
	}
	fill_requests(dongle_1, dongle_2, coder);
	while (check_simulation(coder) == 0)
	{
		pthread_mutex_lock(&dongle_1->lock);
		pthread_mutex_lock(&dongle_2->lock);
		if (both_ready(dongle_1, dongle_2, coder) == 1)
		{
			reserve_dongles(dongle_1, dongle_2, coder);
			return (1);
		}
		pthread_mutex_unlock(&dongle_2->lock);
		pthread_mutex_unlock(&dongle_1->lock);
		pthread_cond_wait(&coder->args->wake_cond, &coder->args->lock_sim);
	}
	pthread_mutex_unlock(&coder->args->lock_sim);
	return (0);
}

void	drop_dongles(t_dongle *dongle_1, t_dongle *dongle_2, t_coder *coder)
{
	pthread_mutex_lock(&coder->args->lock_sim);

	pthread_mutex_lock(&dongle_1->lock);
	dongle_1->is_available = 1;
	dongle_1->cooldown = get_current_time() + coder->args->dongle_cooldown;
	pthread_mutex_unlock(&dongle_1->lock);

	pthread_mutex_lock(&dongle_2->lock);
	dongle_2->is_available = 1;
	dongle_2->cooldown = get_current_time() + coder->args->dongle_cooldown;
	pthread_mutex_unlock(&dongle_2->lock);

	pthread_mutex_unlock(&coder->args->lock_sim);
}
