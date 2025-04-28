/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:39:52 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/28 14:37:33 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

bool	input_is_ok(int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
		return (false);
	i = 0;
	while (++i < ac)
		if (!is_number(av[i]))
			return (false);
	return (true);
}

t_philo	*init_philo(int ac, char **av)
{
	t_philo	*philo;

	(void)ac, (void)av;
	philo = malloc(sizeof(t_philo));
	philo->keep_eating = true;
	philo->n_meals_set = true;
	philo->nbr_philo = ft_atoi(av[1]);
	if (philo->nbr_philo < 1)
		free_philo(philo, "Error: invalid number of philosophers");
	philo->t_die = ft_atoi(av[2]);
	if (philo->t_die < 1)
		free_philo(philo, "Error: invalid time to die");
	philo->t_eat = ft_atoi(av[3]);
	if (philo->t_eat < 1)
		free_philo(philo, "Error: invalid time to eat");
	philo->t_sleep = ft_atoi(av[4]);
	if (philo->t_sleep < 1)
		free_philo(philo, "Error: invalid time to sleep");
	if (ac == 6)
		philo->n_meals = ft_atoi(av[5]);
	else
		philo->n_meals_set = false;
	if (philo->n_meals < -1)
		free_philo(philo, "Error: invalid number of meals");
	return (philo);
}
