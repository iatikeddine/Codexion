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

	int	i = 0;
	while(i < args.number_of_coders)
	{
		printf("coder id: %d\n%d and %d\n\n",
			args.coders[i].coder_id, args.coders[i].left_dongle->dongle_id,
			args.coders[i].right_dongle->dongle_id);
		i++;
	}
}