/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:42:33 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/17 15:27:26 by abidaux          ###   ########.fr       */
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

/**
 * @brief Gets the current system time in milliseconds
 *
 * Uses gettimeofday() to retrieve the current system time and converts it
 * to milliseconds. This is used throughout the program to track elapsed time
 * for philosophers' actions, meal timings, and simulation duration.
 *
 * @return long The current time in milliseconds since the Unix epoch
 */
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief A more accurate implementation of usleep for millisecond precision
 *
 * Standard usleep() can be imprecise for the requirements of this project.
 * This implementation creates a more accurate sleep function by actively
 * checking the elapsed time in a loop. It sleeps in small increments (500μs)
 * and continuously checks if the desired sleep duration has elapsed.
 *
 * @param ms The duration to sleep in milliseconds
 */
void	better_usleep(long long ms)
{
	long	start_time;

	start_time = get_time_ms();
	while (get_time_ms() < start_time + ms)
		usleep(500);
}
