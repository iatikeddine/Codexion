/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 08:28:53 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/13 09:01:40 by iatikedd         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../inc/codexion.h"

void	free_all(t_arguments *args)
{
	if (args->coders)
		free(args->coders);
	if (args->dongles)
		free(args->dongles);
}

void	change_status(t_coder *coder)
{
	print_status("is compiling", coder);
	pthread_mutex_lock(&coder->args->state_lock);
	coder->last_compile_time = get_current_time();
	coder->number_of_compiles++;
	pthread_mutex_unlock(&coder->args->state_lock);
	usleep(coder->args->time_to_compile * 1000);
}
