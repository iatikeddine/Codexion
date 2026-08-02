/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <iatikedd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:00:29 by iatikedd          #+#    #+#             */
/*   Updated: 2026/07/20 13:00:30 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid_number(const char *num)
{
	size_t	i;

	i = 0;
	if (num[0] == '\0')
		return (0);
	if (num[0] == '+' && num[1] != '\0')
		i++;
	while (num[i] != '\0')
	{
		if (num[i] < '0' || num[i] > '9')
		{
			fprintf(stderr, "Error: The argument %s is not valid\n", num);
			return (0);
		}
		i++;
	}
	return (1);
}

int	is_all_alpha(const char *str)
{
	int	i;

	i = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (!(str[i] >= 'a' && str[i] <= 'z')
			&& !(str[i] >= 'A' && str[i] <= 'Z'))
		{
			fprintf(stderr, "Error: invalid scheduler\n");
			return (0);
		}
		i++;
	}
	return (1);
}

void	to_lower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
}

int	str_trim(char **ptr)
{
	int		i;
	size_t	len;

	i = 0;
	if ((*ptr)[0] == '\0')
		return (0);
	while ((*ptr)[i] == ' ' || ((*ptr)[i] >= 9 && (*ptr)[i] <= 13))
		i++;
	if ((*ptr)[i] == '\0')
		return (0);
	*ptr += i;
	len = strlen(*ptr);
	while (len > 0 && ((*ptr)[len - 1] == ' '
		|| ((*ptr)[len - 1] >= 9 && (*ptr)[len - 1] <= 13)))
		len--;
	(*ptr)[len] = '\0';
	return (1);
}
