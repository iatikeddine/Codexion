/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:51:10 by iatikedd          #+#    #+#             */
/*   Updated: 2026/05/12 09:53:07 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"

int	main(int argc, char **argv)
{
	t_arguments args;

	if (argc == 9)
	{
		if (parse_args(argc, argv) == 0)
			return 1;
	}
	else
	{
		fprintf(stderr, "Error: invalid number of arguments\n");
		return 2;
	}
	if (fill_input_data(&args, argv) == 0)
		return 3; // needs error message
	if (sim_init(&args) == 0)
	{
		free_all(&args);
		return 4;
	}
	args.starting_time = get_current_time();
	threads_init(&args);
	pthread_t monitor;
	pthread_create(&monitor, NULL, routine_monitoring, &args);
	join_threads(&args);
	pthread_join(monitor, NULL);
	free_all(&args);
}