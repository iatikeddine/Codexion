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

#include "codexion.h"

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
}