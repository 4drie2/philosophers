/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:53:25 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/28 14:30:40 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * nombre de philosophe >= 1
 *  */


void	free_philo(t_philo *philo, char *msg_error)
{
	if (!philo)
		return ;
	free(philo);
	if (msg_error)
		perror(msg_error);
}

void	print_philo(t_philo *philo)
{
	if (philo)
	{
		printf("Philosopher: %d\n", philo->nbr_philo);
		printf("Time to die: %d\n", philo->t_die);
		printf("Time to eat: %d\n", philo->t_eat);
		printf("Time to sleep: %d\n", philo->t_sleep);
		printf("Number of meals: %d\n", philo->n_meals);
	}
}

int	main(int ac, char **av)
{
	t_philo	*philo;

	(void)ac, (void)av;
	if (!input_is_ok(ac, av))
		return ((void)perror("Error: invalid input. Usage: "
				"./philo nbr_philo t_die t_eat t_sleep [n_meals]\n"), 0);
	philo = init_philo(ac, av);
	print_philo(philo);
	free_philo(philo, NULL);
	return (0);
}
