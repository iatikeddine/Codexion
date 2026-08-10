/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:41 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:00:42 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coders_init(t_arguments *args)
{
	int	i;
	int	last_dongle;

	i = 0;
	while (i < args->number_of_coders)
	{
		args->coders[i].coder_id = i + 1;
		args->coders[i].number_of_compiles = 0;
		args->coders[i].args = args;
		if (i == 0)
		{
			last_dongle = args->number_of_coders - 1;
			args->coders[i].left_dongle = &args->dongles[last_dongle];
			args->coders[i].right_dongle = &args->dongles[i];
			i++;
		}
		else if (i != 0)
		{
			args->coders[i].left_dongle = &args->dongles[i - 1];
			args->coders[i].right_dongle = &args->dongles[i];
			i++;
		}
	}
}

static int	dongles_init(t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		args->dongles[i].dongle_id = i + 1;
		args->dongles[i].is_available = 1;
		args->dongles[i].cooldown = 0;
		args->dongles[i].queue.current_size = 0;
		if (pthread_mutex_init(&args->dongles[i].lock, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&args->dongles[i].lock);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	*allocate_mem(size_t size)
{
	void	*mem;

	mem = malloc(size);
	if (mem == NULL)
	{
		fprintf(stderr, "Error: memory allocation failed\n");
		return (NULL);
	}
	return (mem);
}

static int	init_mutexes(t_arguments *args)
{
	if (pthread_mutex_init(&args->print_lock, NULL) != 0)
		return (fprintf(stderr, "Error: failed to init mutex"), 0);
	if (pthread_mutex_init(&args->state_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&args->print_lock);
		return (fprintf(stderr, "Error: failed to init mutex"), 0);
	}
	if (pthread_mutex_init(&args->lock_sim, NULL) != 0)
	{
		pthread_mutex_destroy(&args->print_lock);
		pthread_mutex_destroy(&args->state_lock);
		return (fprintf(stderr, "Error: failed to init mutex"), 0);
	}
	return (1);
}

int	sim_init(t_arguments *args)
{
	args->ending_point = 'f';
	args->coders = (allocate_mem(sizeof(t_coder)
				* args->number_of_coders));
	if (args->coders == NULL)
		return (fprintf(stderr, "Error: failed to allocate memory\n"), 0);
	args->dongles = (allocate_mem(sizeof(t_dongle)
				* args->number_of_coders));
	if (args->dongles == NULL)
		return (fprintf(stderr, "Error: failed to allocate memory\n"), 0);
	if (pthread_cond_init(&args->wake_cond, NULL) != 0)
		return (fprintf(stderr, "Error: failed to init cond variable"), 0);
	if (init_mutexes(args) == 0)
	{
		pthread_cond_destroy(&args->wake_cond);
		return (0);
	}
	if (dongles_init(args) == 0)
	{
		pthread_mutex_destroy(&args->print_lock);
		pthread_mutex_destroy(&args->state_lock);
		pthread_cond_destroy(&args->wake_cond);
		fprintf(stderr, "Error: failed to init mutex or cond variable");
		return (0);
	}
	coders_init(args);
	return (1);
}
