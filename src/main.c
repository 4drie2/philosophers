/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:53:25 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/30 12:25:06 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * nombre de philosophe >= 1
 *  */

/**
 * better_usleep - Une implémentation plus précise de usleep
 * @param ms: Temps d'attente en millisecondes
 *
 * Cette fonction utilise gettimeofday pour mesurer le temps écoulé
 * avec précision et fait de courtes pauses avec usleep pour ne pas
 * surcharger le CPU.
 */
void	better_usleep(long long ms)
{
	struct timeval	start;
	struct timeval	current;
	long long		elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(50);
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000
				+ (current.tv_usec - start.tv_usec) / 1000;
		if (elapsed >= ms)
			break;
	}
}

void	free_philo(t_rules *philo, char *msg_error)
{
	if (!philo)
		return ;
	free(philo);
	if (msg_error)
		perror(msg_error);
}

void	print_philo(t_rules *philo)
{
	if (philo)
	{
		printf("Philosopher: %d\n", philo->nbr_philo);
		printf("Time to die: %dms\n", philo->t_die);
		printf("Time to eat: %dms\n", philo->t_eat);
		printf("Time to sleep: %dms\n", philo->t_sleep);
		if (philo->n_meals_set)
			printf("Number of meals: %d\n", philo->n_meals);
	}
}

int	main(int ac, char **av)
{
	t_rules	*philo;

	if (!input_is_ok(ac, av))
		return ((void)write(2, "Error: invalid input. Usage: "
				"./philo nbr_philo t_die t_eat t_sleep [n_meals]\n", 77), 0);
	philo = init_philo(ac, av);
	print_philo(philo);
	free_philo(philo, NULL);
	return (0);
}
