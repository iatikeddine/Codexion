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

	if (argc == 9)
	{
		if (parse_args(argc, argv) == 0)
			return 1;
	}
	else
		return (fprintf(stderr, "Error: invalid number of arguments\n"), 2);
	if (fill_input_data(&args, argv) == 0)
		return 3;
	if (args.number_of_coders < 2)
		return (fprintf(stderr, "Error: Invalid number of coders\n"), 4);
	if (sim_init(&args) == 0)
		return (free_all(&args), 5);
	args.starting_time = get_current_time();
	if (threads_init(&args) == 0)
		return (6);
	if (pthread_create(&monitor, NULL, routine_monitoring, &args) != 0)
		return (fprintf(stderr, "Error: failed to create monitor thread\n"), 7);
	if (join_threads(&args) == 0)
		return (8);
	if (pthread_join(monitor, NULL) != 0)
		return (fprintf(stderr, "Error: failed to join monitor thread\n"), 9);
	destroy_mutexes(&args);
	free_all(&args);
}
