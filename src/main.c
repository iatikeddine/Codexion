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

void	destroy_mutexes(t_arguments *args)
{
	int	i;

	pthread_mutex_destroy(&args->print_lock);
	pthread_mutex_destroy(&args->state_lock);

	i = 0;
	while (i < args->number_of_coders)
	{
		pthread_mutex_destroy(&args->dongles[i].lock);
		pthread_cond_destroy(&args->wait_cond);
		i++;
	}
}

void	free_all(t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		free(args->coders[i]);
		free(args->dongles[i]);
		i++;
	}
	free(args->coders);
	free(args->dongles);
}

int	main(int argc, char **argv){

	t_arguments args;
	int	i;

	if (argc == 9){
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
		return 4;

	pthread_t coder[args.number_of_coders];

	args.starting_time = get_current_time() + 100;

	// initializing the coders threads
	i = 0;
	while (i < args.number_of_coders)
	{
		args.coders[i].last_compile_time = get_current_time();
		pthread_create(&coder[i], NULL, routine, &args.coders[i]);
		i++;
	}

	pthread_t monitor;
	pthread_create(&monitor, NULL, routine_monitoring, &args);

	// joining the coders threads
	i = 0;
	while (i < args.number_of_coders)
	{
		pthread_join(coder[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}