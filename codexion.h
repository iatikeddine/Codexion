/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:18:47 by iatikedd          #+#    #+#             */
/*   Updated: 2026/05/07 11:48:06 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

int	is_valid_number(const char *str);
int     is_all_alpha(const char *str);
void    to_lower(char *str);
int	str_trim(char **ptr);

typedef struct	s_arguments{
	size_t	number_of_coders;
	size_t	number_of_compiles_required;

	size_t	time_to_burnout;
	size_t	time_to_compile;
	size_t	time_to_debug;
	size_t	time_to_refactor;
	size_t	dongle_cooldown;
	char	scheduler;

}t_arguments;

#endif
