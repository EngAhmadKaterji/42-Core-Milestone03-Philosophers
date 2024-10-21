/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaterji <akaterji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 09:41:54 by akaterji          #+#    #+#             */
/*   Updated: 2024/08/25 18:05:54 by akaterji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_check_philo_died(t_sim *sim)
{
	int	died;

	pthread_mutex_lock(&sim->state_mutex);
	died = sim->philo_died;
	pthread_mutex_unlock(&sim->state_mutex);
	return (died);
}

int	ft_lock_fork(pthread_mutex_t *fork, t_sim *sim)
{
	pthread_mutex_lock(fork);
	if (ft_check_philo_died(sim))
	{
		pthread_mutex_unlock(fork);
		return (0);
	}
	return (1);
}

int	ft_take_forks(t_philo *philo, t_sim *sim)
{
	while (1)
	{
		if (ft_check_philo_died(sim))
			return (0);
		if (!ft_lock_fork(philo->left_fork, sim))
			return (0);
		if (sim->num_philos > 1 && !ft_lock_fork(philo->right_fork, sim))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
		else if (sim->num_philos == 1)
		{
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
		ft_print_message(sim, philo->id, "has taken left fork");
		ft_print_message(sim, philo->id, "has taken right fork");
		return (1);
	}
}

void	ft_eat_philo(t_philo *philo, t_sim *sim, int *iterations)
{
	pthread_mutex_lock(&sim->state_mutex);
	gettimeofday(&philo->last_meal_time, NULL);
	ft_print_message(sim, philo->id, "is eating");
	pthread_mutex_unlock(&sim->state_mutex);
	custom_usleep(sim->time_to_eat * 1000, philo);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&sim->state_mutex);
	(*iterations)++;
	philo->meals_eaten = *iterations;
	pthread_mutex_unlock(&sim->state_mutex);
}

void	ft_sleep_philo(t_philo *philo, t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	if (!sim->philo_died)
		ft_print_message(sim, philo->id, "is sleeping");
	pthread_mutex_unlock(&sim->state_mutex);
	custom_usleep(sim->time_to_sleep * 1000, philo);
}
