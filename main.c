/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:51:10 by iatikedd          #+#    #+#             */
/*   Updated: 2026/05/06 11:47:11 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// returns an array of input numbers
static int	fill_input_data(t_arguments *data, char **argv){
	data->number_of_coders = atoi(argv[1]);
	data->time_to_burnout = atoi(argv[2]);
	data->time_to_compile = atoi(argv[3]);
	data->time_to_debug = atoi(argv[4]);
	data->time_to_refactor = atoi(argv[5]);
	data->number_of_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = 'F';
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = 'E';
	return 0;
}

int	main(int argc, char **argv){
	if (argc != 9){
		fprintf(stderr, "Error: Invalid number of arguments");
		return 1;
	}
	t_arguments input_data;
	int	i = 1;

	while (i < argc - 1)
	{
		if (is_valid_number(argv[i]) == 0)
		{
			fprintf(stderr, "Error: The argument %s is not valid", argv[i]);
			return 2;
		}
		i++;
	}
	fill_input_data(&input_data, argv);
	printf("%c\n", input_data.scheduler);
}
