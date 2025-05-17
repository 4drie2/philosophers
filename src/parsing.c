/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:39:52 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/17 15:25:15 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

bool	input_is_ok(int ac, char **av)
{
	int		i;
	long	value;

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

int	init_rules(t_rules **rules, int ac, char **av)
{
	(void)ac, (void)av;
	(*rules) = malloc(sizeof(t_rules));
	if (!(*rules))
		return (0);
	(*rules)->keep_eating = true;
	(*rules)->n_meals_set = true;
	(*rules)->nbr_philo = ft_atoi(av[1]);
	(*rules)->t_die = ft_atoi(av[2]);
	(*rules)->t_eat = ft_atoi(av[3]);
	(*rules)->t_sleep = ft_atoi(av[4]);
	if (ac == 6)
		(*rules)->n_meals = ft_atoi(av[5]);
	else
		(*rules)->n_meals_set = false;
	return (1);
}

/**
 * init_forks - Initialise les mutex pour les fourchettes
 * @param rules: Structure contenant les règles
 * @return: Tableau de mutex initialisés, NULL en cas d'erreur
 */
int	init_forks(pthread_mutex_t **forks, t_rules *rules)
{
	int				i;

	(*forks) = malloc(sizeof(pthread_mutex_t) * rules->nbr_philo);
	if (!(*forks))
		return (0);
	i = -1;
	while (++i < rules->nbr_philo)
	{
		if (pthread_mutex_init(&(*forks)[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(*forks)[i]);
			free((*forks));
			return (0);
		}
	}
	return (1);
}

/**
 * init_philos - Initialise les philosophes
 * @param rules: Structure contenant les règles
 * @param forks: Tableau de mutex pour les fourchettes
 * @return: Tableau de philosophes initialisés, NULL en cas d'erreur
 */
int	init_philos(t_philo **philos, t_rules *rules, pthread_mutex_t *forks)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * rules->nbr_philo);
	if (!*philos)
		return (0);
	i = -1;
	while (++i < rules->nbr_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].n_meals_eaten = 0;
		(*philos)[i].last_meal_time = 0;
		(*philos)[i].rules = rules;
		(*philos)[i].left_fork = &forks[i];
		if (i == rules->nbr_philo - 1)
			(*philos)[i].right_fork = &forks[0];
		else
			(*philos)[i].right_fork = &forks[i + 1];
	}
	return (1);
}
