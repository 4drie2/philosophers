/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:53:25 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/22 10:14:43 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/* tester : https://github.com/MichelleJiam/LazyPhilosophersTester */

/**
 * @brief Checks if all philosophers have eaten enough meals
 *
 * Verifies whether all philosophers have reached the required number of
 * meals (if specified). Used to determine if the simulation should end
 * due to all philosophers being satisfied.
 *
 * @param rules Pointer to the rules structure containing simulation data
 * @return true if all philosophers ate the required number of meals
 * @return false if meals requirement is not set or not all have eaten enough
 */
bool	all_ate_enough(t_rules *rules)
{
	int		i;
	bool	result;

	if (!rules->n_meals_set)
		return (false);
	result = true;
	i = -1;
	pthread_mutex_lock(&rules->meal_mutex);
	while (++i < rules->nbr_philo)
	{
		if (rules->philos[i].n_meals_eaten < rules->n_meals)
		{
			result = false;
			break ;
		}
	}
	pthread_mutex_unlock(&rules->meal_mutex);
	return (result);
}

/**
 * @brief Checks if any philosopher has died from starvation
 *
 * Iterates through all philosophers and verifies if any has exceeded the
 * maximum time allowed without eating (time_to_die). If one has died,
 * the function prints the death message and updates the simulation state
 * to stop all threads.
 *
 * @param rules Pointer to the rules structure containing simulation data
 * @return true if a philosopher has died, false otherwise
 */
static bool	check_philo_death(t_rules *rules)
{
	int		i;
	long	current_time;

	i = -1;
	while (++i < rules->nbr_philo)
	{
		pthread_mutex_lock(&rules->meal_mutex);
		current_time = get_time_ms();
		if (current_time - rules->philos[i].last_meal_time > rules->t_die)
		{
			pthread_mutex_unlock(&rules->meal_mutex);
			pthread_mutex_lock(&rules->print_mutex);
			printf("%5ld %3d died\n", current_time - rules->start_time,
				rules->philos[i].id);
			pthread_mutex_unlock(&rules->print_mutex);
			pthread_mutex_lock(&rules->state_mutex);
			rules->keep_eating = false;
			pthread_mutex_unlock(&rules->state_mutex);
			return (true);
		}
		pthread_mutex_unlock(&rules->meal_mutex);
	}
	return (false);
}

/**
 * @brief Thread routine for monitoring philosophers' state
 *
 * Continuously checks if any philosopher has died or if all philosophers
 * have eaten enough times (when meals limit is specified).
 * Terminates the simulation by updating the keep_eating flag when either
 * condition is met.
 *
 * @param arg Pointer to the rules structure (cast to void*)
 * @return NULL when monitoring is complete
 */
void	*monitor_routine(void *arg)
{
	t_rules	*rules;

	rules = (t_rules *)arg;
	while (1)
	{
		if (check_philo_death(rules))
			return (NULL);
		if (all_ate_enough(rules))
		{
			pthread_mutex_lock(&rules->state_mutex);
			rules->keep_eating = false;
			pthread_mutex_unlock(&rules->state_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

/**
 * @brief Initiates and manages the philosophers simulation
 *
 * Creates threads for each philosopher and a monitor thread to observe
 * the simulation state. Sets the initial timestamps and waits for all
 * threads to complete their execution before returning.
 *
 * The function handles:
 * - Setting the simulation start time
 * - Initializing philosophers' last meal times
 * - Creating philosopher threads with philo_routine
 * - Creating a monitor thread to check for death or completion
 * - Joining all threads when the simulation ends
 *
 * @param rules Pointer to the rules structure containing simulation data
 * @return 1 on successful simulation completion, 0 if thread creation fails
 */
int	start_simulation(t_rules *rules)
{
	int			i;
	pthread_t	monitor;

	rules->start_time = get_time_ms();
	rules->keep_eating = true;
	i = -1;
	while (++i < rules->nbr_philo)
	{
		rules->philos[i].last_meal_time = rules->start_time;
		if (pthread_create(&rules->philos[i].thread, NULL,
				philo_routine, &rules->philos[i]))
			return (0);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, rules))
		return (0);
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < rules->nbr_philo)
		pthread_join(rules->philos[i].thread, NULL);
	return (1);
}

int	main(int ac, char **av)
{
	t_rules	*rules;

	if (!input_is_ok(ac, av))
		return ((void)write(2, "Error: invalid input. Usage: "
				"./philo nbr_philo t_die t_eat t_sleep [n_meals]\n", 77), 0);
	if (!init_rules(&rules, ac, av))
		return (1);
	if (!init_forks(&rules->forks, rules))
		return (clean_up(rules, "Error: Could not initialize forks\n"), 1);
	if (!init_philos(&rules->philos, rules, rules->forks))
		return (clean_up(rules, "Error:"
				"Could not initialize philosophers\n"), 1);
	if (pthread_mutex_init(&rules->print_mutex, NULL)
		|| pthread_mutex_init(&rules->meal_mutex, NULL)
		|| pthread_mutex_init(&rules->state_mutex, NULL))
		return (clean_up(rules, "Error: Could not initialize mutexes\n"), 1);
	if (!start_simulation(rules))
		return (clean_up(rules, "error with start_simulation"), 1);
	clean_up(rules, NULL);
	return (0);
}
