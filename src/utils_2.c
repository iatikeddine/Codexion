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
