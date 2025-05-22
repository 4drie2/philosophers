/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:18:01 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/22 10:38:29 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/**
 * @brief Prints the current status of a philosopher
 *
 * Safely outputs status messages to the console by using a mutex to prevent
 * interleaved outputs from multiple threads. The format follows the project
 * requirements: timestamp (ms) philosopher_id status
 * Output only occurs if the simulation is still ongoing (keep_eating is true).
 *
 * @param philo Pointer to the philosopher's data structure
 * @param status String describing the philosopher's current action
 */
void	print_status(t_philo *philo, char *status)
{
	long	current_time;

	pthread_mutex_lock(&philo->rules->print_mutex);
	pthread_mutex_lock(&philo->rules->state_mutex);
	if (philo->rules->keep_eating)
	{
		current_time = get_time_ms() - philo->rules->start_time;
		printf("%5ld %3d %s\n", current_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->rules->state_mutex);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

/**
 * @brief Attempts to lock a fork mutex while checking simulation state
 *
 * This function tries to acquire a lock on a fork while ensuring the simulation
 * is still running. It checks two main conditions:
 * 1. If the simulation should continue (keep_eating flag)
 * 2. If there are enough philosophers for the dining problem to be valid
 *
 * @param fork The mutex representing the fork to be locked
 * @param philo Pointer to the philosopher's data structure
 * @return true if the fork was successfully locked, false otherwise
 */
static bool	lock_fork(pthread_mutex_t *fork, t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->state_mutex);
	if (!philo->rules->keep_eating)
		return (pthread_mutex_unlock(&philo->rules->state_mutex), false);
	pthread_mutex_unlock(&philo->rules->state_mutex);
	if (philo->rules->nbr_philo <= 1)
		return (false);
	if (pthread_mutex_lock(fork) != 0)
		return (false);
	return (true);
}

/**
 * @brief Allows a philosopher to take both forks needed for eating
 *
 * Implements a deadlock prevention strategy by having even and odd-numbered
 * philosophers take forks in opposite orders. Even-numbered philosophers
 * take the right fork first, then the left, while odd-numbered philosophers
 * take the left fork first, then the right. This breaks the circular wait
 * condition necessary for deadlocks to occur.
 *
 * @param philo Pointer to the philosopher's data structure
 */
bool	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		if (!lock_fork(philo->right_fork, philo))
			return (false);
		print_status(philo, "has taken a fork");
		if (!lock_fork(philo->left_fork, philo))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (false);
		}
	}
	else
	{
		if (!lock_fork(philo->left_fork, philo))
			return (false);
		print_status(philo, "has taken a fork");
		if (!lock_fork(philo->right_fork, philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (false);
		}
	}
	print_status(philo, "has taken a fork");
	return (true);
}

/**
 * @brief Simulates a philosopher eating
 *
 * Updates the philosopher's last meal time and increments the meal counter.
 * These operations are protected by a mutex to ensure data consistency
 * across threads. After updating the data, it prints the eating status
 * and makes the thread sleep for the eating duration specified in the rules.
 *
 * @param philo Pointer to the philosopher's data structure
 */
void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->meal_mutex);
	philo->last_meal_time = get_time_ms();
	philo->n_meals_eaten++;
	pthread_mutex_unlock(&philo->rules->meal_mutex);
	print_status(philo, "is eating");
	better_usleep(philo->rules->t_eat);
}

/**
 * @brief The main routine executed by each philosopher thread
 *
 * This function implements the philosopher's life cycle: thinking, taking forks,
 * eating, and sleeping. Even-numbered philosophers start with a small delay to
 * reduce the risk of deadlocks. The routine continues until the simulation ends
 * (when a philosopher dies or all have eaten enough).
 *
 * @param arg Pointer to the philosopher's data structure (cast to void*)
 * @return NULL when the routine is complete
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		better_usleep(10);
	while (1)
	{
		pthread_mutex_lock(&philo->rules->state_mutex);
		if (!philo->rules->keep_eating)
		{
			pthread_mutex_unlock(&philo->rules->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->rules->state_mutex);
		if (take_forks(philo))
		{
			eat(philo);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			print_status(philo, "is sleeping");
			better_usleep(philo->rules->t_sleep);
			print_status(philo, "is thinking");
		}
	}
	return (NULL);
}
