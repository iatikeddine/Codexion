/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iatikedd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:51:00 by iatikedd          #+#    #+#             */
/*   Updated: 2026/05/06 14:20:03 by iatikedd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid_number(const char *num)
{
	if (num[0] == '\0')
		return 0;
	size_t	i = 0;

	while (num[i] != '\0')
	{
		if (num[i] < '0' || num[i] > '9')
			return 0;
		i++;
	}
	return 1;
}

int	is_alpha(char *str)
{
	size_t	i = 0;

	if (str[0] == '\0')
		return 0;
	while(str[i])
	{
		if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z'))
			return 0;
		i++;
	}
	return 1;
}

int main()
{
	char *str = "edf";
	printf("%d\n", is_alpha(str));
}
