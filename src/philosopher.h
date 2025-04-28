/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:54:24 by abidaux           #+#    #+#             */
/*   Updated: 2025/04/28 14:36:48 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct s_philo
{
	bool	keep_eating;
	int		nbr_philo;
	int		t_die;
	int		t_eat;
	int		t_sleep;
	bool	n_meals_set;
	int		n_meals;
} t_philo;

bool	input_is_ok(int ac, char **av);
t_philo	*init_philo(int ac, char **av);
bool	is_number(char *number);
int		ft_atoi(char *nbr);
void	free_philo(t_philo *philo, char *msg_error);
