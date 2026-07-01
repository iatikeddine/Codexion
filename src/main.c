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

int	main(int argc, char **argv){

	t_arguments args;
	int	i;

	if (argc == 9){
		if (parse_args(argc, argv, &args) == 0)
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
		return 4;

	args.starting_time = get_current_time() + 50;
	pthread_t monitor;

	pthread_create(&monitor, NULL, routine_monitoring, &args);

	pthread_t coder[args.number_of_coders];
	i = 0;
	while (i < args.number_of_coders)
	{
		pthread_create(&coder[i], NULL, routine, &args.coders[i]);
		i++;
	}

	i = 0;
	while (i < args.number_of_coders)
	{
		pthread_join(coder[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}