/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 10:07:40 by iatikedd          #+#    #+#             */
/*   Updated: 2026/05/13 10:07:41 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void    coder_init(t_arguments *args)
{
    int i;

    i = 0;
    while (i < args->number_of_coders)
    {
        args->coders[i].coder_id = i;
        args->coders[i].number_of_compiles = 0;
        args->coders[i].ags = args;
        if (i == 0)
        {
            int last_dongle;

            last_dongle = args->number_of_coders - 1;
            args->coders[i].left_dongle = &args->dongles[i + last_dongle].dongle;
            args->coders[i].right_dongle = &args->dongles[i].dongle;
            i++;
        }
        else if (i != 0)
        {
            args->coders[i].left_dongle = &args->dongles[i - 1].dongle;
            args->coders[i].right_dongle = &args->dongles[i].dongle;
            i++;
        }
    }

}

void    *allocate_mem(size_t size)
{
    void    *mem;

    mem = malloc(size);
    if (mem == NULL)
    {   
        fprintf(stderr, "Error: memory allocation failed\n");
        return NULL;
    }
    return mem;
}

int sim_init(t_arguments *args)
{
    int i;

    i = 0;
    args->ending_point = 'f';
    args->coders = (t_coder *)(allocate_mem(sizeof(t_coder) * args->number_of_coders));
    if (args->coders == NULL)
        return 0; // must follow return value and exit
    args->dongles = (t_dongle *)(allocate_mem(sizeof(t_dongle) * args->number_of_coders));
    if (args->dongles == NULL)
        return 0; // must follow return value and exit

    while (i < args->number_of_coders)
    {
        pthread_mutex_init(&args->dongles[i].dongle, NULL);
        args->dongles[i].dongle_id = i;
        i++;
    }
}