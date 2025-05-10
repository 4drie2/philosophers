/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:53:25 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/10 13:13:08 by abidaux          ###   ########.fr       */
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
 * free_rules - Libère toutes les ressources allouées
 * @param rules: Structure contenant les règles
 * @param msg_error: Message d'erreur à afficher
 */
void	free_rules(t_rules *rules, char *msg_error)
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
	t_philo			*philos;
	t_rules			*rules;
	pthread_mutex_t	*forks;

	if (!input_is_ok(ac, av))
		return ((void)write(2, "Error: invalid input. Usage: "
				"./philo nbr_philo t_die t_eat t_sleep [n_meals]\n", 77), 0);
	if (!init_rules(&rules, ac, av))
		return(write(2, "error with init rules\n", 22), 1);
	if (!init_forks(&forks, rules))
		return (free_rules(rules, "error with init forks"), 1);
	rules->forks = forks;
	if (!init_philos(&philos, rules, forks))
		return (free_rules(rules, "error with init philo"), 1);
	rules->philos = philos;
	free_rules(rules, NULL);
	return (0);
}

