/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:53:25 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/16 20:34:10 by abidaux          ###   ########.fr       */
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

// int	main(int ac, char **av)
// {
// 	t_philo			*philos;
// 	t_rules			*rules;
// 	pthread_mutex_t	*forks;

// 	if (!input_is_ok(ac, av))
// 		return ((void)write(2, "Error: invalid input. Usage: "
// 				"./philo nbr_philo t_die t_eat t_sleep [n_meals]\n", 77), 0);
// 	if (!init_rules(&rules, ac, av))
// 		return(write(2, "error with init rules\n", 22), 1);
// 	if (!init_forks(&forks, rules))
// 		return (free_rules(rules, "error with init forks"), 1);
// 	rules->forks = forks;
// 	if (!init_philos(&philos, rules, forks))
// 		return (free_rules(rules, "error with init philo"), 1);
// 	rules->philos = philos;
// 	free_rules(rules, NULL);
// 	return (0);
// }

#include "philosopher.h"

// Fonction pour obtenir le temps actuel en millisecondes
long get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// Fonction pour l'attente précise
void better_usleep(long long ms)
{
	long start_time;

	start_time = get_time_ms();
	while (get_time_ms() < start_time + ms)
		usleep(500);
}

// Affichage du statut d'un philosophe avec timestamp
void print_status(t_philo *philo, char *status)
{
	long current_time;

	pthread_mutex_lock(&philo->rules->print_mutex);
	if (philo->rules->keep_eating)
	{
		current_time = get_time_ms() - philo->rules->start_time;
		printf("%ld %d %s\n", current_time, philo->id, status);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

// Prise des fourchettes
void take_forks(t_philo *philo)
{
	// Philosophes pairs et impairs prennent les fourchettes dans l'ordre inverse
	// pour éviter les deadlocks
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

// Reposer les fourchettes
void put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

// Manger
void eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->meal_mutex);
	philo->last_meal_time = get_time_ms();
	philo->n_meals_eaten++;
	pthread_mutex_unlock(&philo->rules->meal_mutex);

	print_status(philo, "is eating");
	better_usleep(philo->rules->t_eat);
}

// Routine des philosophes
void *philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	// Si philosophe pair, attend un peu pour éviter les deadlocks
	if (philo->id % 2 == 0)
		better_usleep(10);
	while (1)
	{
		pthread_mutex_lock(&philo->rules->state_mutex);
		if (!philo->rules->keep_eating)
		{
			pthread_mutex_unlock(&philo->rules->state_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->rules->state_mutex);
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		print_status(philo, "is sleeping");
		better_usleep(philo->rules->t_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

// Vérifier si tous les philosophes ont mangé assez
bool all_ate_enough(t_rules *rules)
{
	int i;
	bool result;

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
			break;
		}
	}
	pthread_mutex_unlock(&rules->meal_mutex);
	return (result);
}

void *monitor_routine(void *arg)
{
	t_rules	*rules;
	int		i;
	long	current_time;

	rules = (t_rules *)arg;
	while (1)
	{
		i = -1;
		while (++i < rules->nbr_philo)
		{
			pthread_mutex_lock(&rules->meal_mutex);
			current_time = get_time_ms();
			if (current_time - rules->philos[i].last_meal_time > rules->t_die)
			{
				pthread_mutex_unlock(&rules->meal_mutex);
				pthread_mutex_lock(&rules->print_mutex);
				printf("%ld %d died\n", current_time - rules->start_time, rules->philos[i].id);
				pthread_mutex_unlock(&rules->print_mutex);
				pthread_mutex_lock(&rules->state_mutex);
				rules->keep_eating = false;
				pthread_mutex_unlock(&rules->state_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&rules->meal_mutex);
		}
		if (all_ate_enough(rules))
		{
			pthread_mutex_lock(&rules->state_mutex);
			rules->keep_eating = false;
			pthread_mutex_unlock(&rules->state_mutex);
			return (NULL);
		}
		usleep(1000); // Attente courte pour ne pas surcharger le CPU
	}
	return (NULL);
}

// Initialisation et démarrage de la simulation
int start_simulation(t_rules *rules)
{
	int i;
	pthread_t monitor;

	rules->start_time = get_time_ms();
	rules->keep_eating = true;

	i = -1;
	while (++i < rules->nbr_philo)
	{
		rules->philos[i].last_meal_time = rules->start_time;
		if (pthread_create(&rules->philos[i].thread, NULL, philo_routine, &rules->philos[i]))
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

// Fonction qui libère la mémoire
void clean_up(t_rules *rules)
{
	int i;

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

// Fonction principale
int main(int ac, char **av)
{
	t_rules *rules;

	if (!input_is_ok(ac, av))
		return ((void)write(2, "Error: invalid input. Usage: "
				"./philo nbr_philo t_die t_eat t_sleep [n_meals]\n", 77), 0);
	if (!init_rules(&rules, ac, av))
		return (1);
	if (!init_forks(&rules->forks, rules))
		return (free_rules(rules, "Error: Could not initialize forks\n"), 1);
	if (!init_philos(&rules->philos, rules, rules->forks))
		return (free_rules(rules, "Error: Could not initialize philosophers\n"), 1);
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0 ||
		pthread_mutex_init(&rules->meal_mutex, NULL) != 0 ||
		pthread_mutex_init(&rules->state_mutex, NULL) != 0)
		return (free_rules(rules, "Error: Could not initialize mutexes\n"), 1);
	if (!start_simulation(rules))
		return (clean_up(rules), 1);
		clean_up(rules);
		return (0);
}
