/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:42:33 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/28 16:38:04 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

bool	is_number(char *number)
{
	int	i;

	i = 0;
	if (number[i] == '-')
		return (false);
	while (number[i])
	{
		if (number[i] < '0' || number[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	ft_atoi(char *nbr)
{
	long	result;
	int		i;
	int		sign;

	i = 0;
	result = 0;
	sign = 1;
	if (nbr[i] == '-')
		sign = -1;
	if (nbr[i] == '+' || nbr[i] == '-')
		i++;
	while (nbr[i] >= '0' && nbr[i] <= '9')
		result = result * 10 + (nbr[i++] - '0');
	return (sign * result);
}
