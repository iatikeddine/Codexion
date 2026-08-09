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

#include "codexion.h"

static int	compiling(t_coder *coder)
{
	if (take_dongles(coder->right_dongle, coder->left_dongle, coder) == 0)
		return 0;
	change_status(coder);
	drop_dongles(coder->right_dongle, coder->left_dongle, coder);
	return 1;
}

static void	debugging(t_coder *coder)
{
	print_status("is debugging", coder);
	smart_sleep(coder->args->time_to_debug, coder);
}

static void	refactoring(t_coder *coder)
{
	print_status("is refactoring", coder);
	smart_sleep(coder->args->time_to_refactor, coder);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	long	sleep;

	coder = (t_coder *)arg;
	sleep = coder->args->time_to_compile + coder->args->dongle_cooldown;
	if (coder->coder_id % 2 == 0)
		smart_sleep(sleep / 3, coder);
	while (check_simulation(coder) == 0)
	{
		if(compiling(coder) == 0)
			return (NULL);
		if (check_simulation(coder) == 1)
			return (NULL);
		debugging(coder);
		if (check_simulation(coder) == 1)
			return (NULL);
		refactoring(coder);
	}
	return (NULL);
}
