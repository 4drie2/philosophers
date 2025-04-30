/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:39:52 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/30 12:27:58 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

bool input_is_ok(int ac, char **av)
{
	int i;
	long value;

	if (ac < 5 || ac > 6)
		return (false);
	i = 0;
	while (++i < ac)
	{
		if (!is_number(av[i]))
			return (false);
		value = ft_atoi(av[i]);
		if (value > INT_MAX || value < INT_MIN)
			return (false);
		if (i <= 4 && value <= 0)
			return (false);
		if (i == 5 && value < 0)
			return (false);
	}
	return (true);
}


t_rules	*init_rules(int ac, char **av)
{
	t_rules	*philo;

	(void)ac, (void)av;
	philo = malloc(sizeof(t_rules));
	philo->keep_eating = true;
	philo->n_meals_set = true;
	philo->nbr_philo = ft_atoi(av[1]);
	philo->t_die = ft_atoi(av[2]);
	philo->t_eat = ft_atoi(av[3]);
	philo->t_sleep = ft_atoi(av[4]);
	if (ac == 6)
		philo->n_meals = ft_atoi(av[5]);
	else
		philo->n_meals_set = false;
	return (philo);
}

/**
 * init_forks - Initialise les mutex pour les fourchettes
 * @param rules: Structure contenant les règles
 * @return: Tableau de mutex initialisés, NULL en cas d'erreur
 */
pthread_mutex_t	*init_forks(t_rules *rules)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * rules->nbr_philo);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < rules->nbr_philo)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			free(forks);
			return (NULL);
		}
		i++;
	}
	return (forks);
}
