/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:45 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:00:47 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"

static int	clean_and_exit(t_arguments *args, int code, char *msg)
{
	if (msg)
		fprintf(stderr, "%s", msg);
	destroy_mutexes(args);
	free_all(args);
	return (code);
}

static int	run_simulation(t_arguments *args)
{
	pthread_t	monitor;

	args->starting_time = get_current_time();
	if (threads_init(args) == 0)
		return (clean_and_exit(args, 6, NULL));
	if (pthread_create(&monitor, NULL, routine_monitoring, args) != 0)
	{
		pthread_mutex_lock(&args->state_lock);
		args->ending_point = 't';
		pthread_mutex_unlock(&args->state_lock);
		join_threads(args);
		return (clean_and_exit(args, 7,
				"Error: failed to create monitor thread\n"));
	}
	if (join_threads(args) == 0)
		return (clean_and_exit(args, 8, NULL));
	if (pthread_join(monitor, NULL) != 0)
		return (clean_and_exit(args, 9,
				"Error: failed to join monitor thread\n"));
	return (clean_and_exit(args, 0, NULL));
}

int	main(int argc, char **argv)
{
	t_arguments	args;

	memset(&args, 0, sizeof(t_arguments));
	if (argc == 9)
	{
		if (parse_args(argc, argv) == 0)
			return (1);
	}
	else
		return (fprintf(stderr, "Error: invalid number of arguments\n"), 2);
	if (fill_input_data(&args, argv) == 0)
		return (3);
	if (args.number_of_coders < 1)
		return (fprintf(stderr, "Error: 1 or more coders needed\n"), 4);
	if (args.number_of_compiles_required == 0)
		return (0);
	if (sim_init(&args) == 0)
		return (free_all(&args), 5);
	return (run_simulation(&args));
}
