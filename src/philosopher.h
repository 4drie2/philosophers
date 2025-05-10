/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:24 by abidaux           #+#    #+#             */
/*   Updated: 2025/05/10 12:56:04 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				n_meals_eaten;
	long			last_meal_time;
	struct s_rules	*rules;
} t_philo;

typedef struct s_rules
{
	bool			keep_eating;
	int				nbr_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	bool			n_meals_set;
	int				n_meals;
	t_philo			*philos;
	pthread_mutex_t	*forks;
} t_rules;

bool			input_is_ok(int ac, char **av);
int				init_rules(t_rules **rules, int ac, char **av);
bool			is_number(char *number);
int				ft_atoi(char *nbr);
void			free_rules(t_rules *rules, char *msg_error);
void			better_usleep(long long ms);
int				init_forks(pthread_mutex_t **forks, t_rules *rules);
int				init_philos(t_philo **philo , t_rules *rules, pthread_mutex_t *forks);

