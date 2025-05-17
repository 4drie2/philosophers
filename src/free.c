/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:13:30 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/17 15:37:19 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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

/**
 * @brief Cleans up all allocated resources at the end of the program
 *
 * This function destroys all mutexes and frees all memory allocated during
 * the program execution, preventing memory leaks and ensuring proper cleanup.
 *
 * @param rules Pointer to the main structure containing all program data
 */
void	clean_up(t_rules *rules)
{
	int	i;

	i = -1;
	while (++i < rules->nbr_philo)
		pthread_mutex_destroy(&rules->forks[i]);
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->meal_mutex);
	pthread_mutex_destroy(&rules->state_mutex);
	free(rules->forks);
	free(rules->philos);
	free(rules);
}
