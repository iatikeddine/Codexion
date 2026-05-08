/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:51:10 by iatikedd          #+#    #+#             */
/*   Updated: 2026/05/07 12:13:55 by iatikedd         ###   ########.fr       */
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
	else
		return 0;
	return 1;
}

int	remove_white_space(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (str_trim(&argv[i]) == 0)
			return 0;
		i++;
	}
	return 1;
}

int	main(int argc, char **argv){
	
	int	i;
	t_arguments input_data;

	i = 1;
	if (argc != 9){
		fprintf(stderr, "Error: Invalid number of arguments");
		return 1;
	}
	if (remove_white_space(argc, argv) == 0)
		return 2;
	while (i < argc - 1)
	{
		if (is_valid_number(argv[i]) == 0)
		{
			fprintf(stderr, "Error: The argument %s is not valid", argv[i]);
			return 3;
		}
		i++;
	}
	to_lower(argv[8]);
	if (is_all_alpha(argv[8]) == 0)
	{
		fprintf(stderr, "Error: invalid scheduler");
		return 4;
	}
	if (fill_input_data(&input_data, argv) == 0)
		return 5;
}
