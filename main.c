/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:51:10 by iatikedd          #+#    #+#             */
/*   Updated: 2026/04/30 11:46:08 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv){
	if (argc != 9){
		fprintf(stderr, "Error: Invalid number of arguments");
		return 1;
	}
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
	printf("All good");
	return 0;
}
