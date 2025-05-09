/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:53:25 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/09 17:39:50 by abidaux          ###   ########.fr       */
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

/**
 * free_forks - Libère les mutex des fourchettes
 * @param rules: Structure contenant les règles
 */
void	free_forks(t_rules *rules)
{
	int	i;

	if (!rules->forks)
		return ;
	i = -1;
	while (++i < rules->nbr_philo)
		pthread_mutex_destroy(&rules->forks[i]);
	free(rules->forks);
}

/**
 * free_philo - Libère toutes les ressources allouées
 * @param rules: Structure contenant les règles
 * @param msg_error: Message d'erreur à afficher
 */
void	free_philo(t_rules *rules, char *msg_error)
{
	if (!rules)
		return ;
	free_forks(rules);
	if (rules->philos)
		free(rules->philos);
	free(rules);
	if (msg_error)
		perror(msg_error);
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_rules	*rules;

	if (!input_is_ok(ac, av))
		return ((void)write(2, "Error: invalid input. Usage: "
				"./philo nbr_philo t_die t_eat t_sleep [n_meals]\n", 77), 0);
	rules = init_rules(ac, av);
	philo = init_philos(rules, init_forks(rules));
	free_philo(philo, NULL);
	return (0);
}
