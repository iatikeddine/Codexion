/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:24 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:00:25 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	remove_white_space(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (str_trim(&argv[i]) == 0)
		{
			fprintf(stderr, "The %dth argument is not valid\n", i);
			return (0);
		}
		i++;
	}
	return (1);
}

static long	ft_atol(const char *str)
{
	int		i;
	int		digit;
	long	result;

	i = 0;
	result = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (result > (LONG_MAX - digit) / 10)
			return (-1);
		result = result * 10 + digit;
		i++;
	}
	return (result);
}

int	fill_input_data(t_arguments *args, char **argv)
{
	args->number_of_coders = ft_atol(argv[1]);
	args->time_to_burnout = ft_atol(argv[2]);
	args->time_to_compile = ft_atol(argv[3]);
	args->time_to_debug = ft_atol(argv[4]);
	args->time_to_refactor = ft_atol(argv[5]);
	args->number_of_compiles_required = ft_atol(argv[6]);
	args->dongle_cooldown = ft_atol(argv[7]);
	if (args->number_of_coders == -1 || args->time_to_burnout == -1
		|| args->time_to_compile == -1 || args->time_to_debug == -1
		|| args->time_to_refactor == -1 || args->dongle_cooldown == -1
		|| args->number_of_compiles_required == -1)
	{
		fprintf(stderr, "Error: numbers must be lower than LONG_MAX\n");
		return (0);
	}
	if (strcmp(argv[8], "fifo") == 0)
		args->scheduler = 'F';
	else if (strcmp(argv[8], "edf") == 0)
		args->scheduler = 'E';
	else
	{
		fprintf(stderr, "Error: invalid scheduler type\n");
		return (0);
	}
	return (1);
}

int	parse_args(int argc, char **argv)
{
	int	i;

	i = 1;
	if (remove_white_space(argc, argv) == 0)
		return (0);
	while (i < argc - 1)
	{
		if (is_valid_number(argv[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}
