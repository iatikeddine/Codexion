/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:51:10 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/13 10:16:46 by iatikedd         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/codexion.h"

int	main(int argc, char **argv)
{
	t_arguments	args;
	pthread_t	monitor;

	memset(&args, 0, sizeof(t_arguments));
	if (argc == 9)
	{
		if (parse_args(argc, argv) == 0)
			return (1);
	}else
		return (fprintf(stderr, "Error: invalid number of arguments\n"), 2);
	if (fill_input_data(&args, argv) == 0)
		return (3);
	if (args.number_of_coders < 1)
		return (fprintf(stderr, "Error: 1 or more coders needed\n"), 4);
	if (sim_init(&args) == 0)
		return (free_all(&args), 5);
	args.starting_time = get_current_time();
	if (threads_init(&args) == 0)
	{
		destroy_mutexes(&args);
		free_all(&args);
		return (6);
	}
	if (pthread_create(&monitor, NULL, routine_monitoring, &args) != 0)
	{
		pthread_mutex_lock(&args.state_lock);
		args.ending_point = 't';
		pthread_mutex_unlock(&args.state_lock);
		join_threads(&args);
		destroy_mutexes(&args);
		free_all(&args);
		return (fprintf(stderr, "Error: failed to create monitor thread\n"), 7);
	}
	if (join_threads(&args) == 0)
	{
		destroy_mutexes(&args);
		free_all(&args);
		return (8);
	}
	if (pthread_join(monitor, NULL) != 0)
	{
		destroy_mutexes(&args);
		free_all(&args);
		return (fprintf(stderr, "Error: failed to join monitor thread\n"), 9);
	}
	destroy_mutexes(&args);
	free_all(&args);
}
