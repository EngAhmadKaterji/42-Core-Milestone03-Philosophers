/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaterji <akaterji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:55:48 by akaterji          #+#    #+#             */
/*   Updated: 2024/08/26 14:07:53 by akaterji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_check_philosopher_deaths(t_sim *sim)
{
	struct timeval	current_time;
	long			elapsed_time;
	int				i;

	i = -1;
	gettimeofday(&current_time, NULL);
	while (++i < sim->num_philos)
	{
		pthread_mutex_lock(&sim->philos[i].state_mutex);
		elapsed_time = (current_time.tv_sec
				- sim->philos[i].last_meal_time.tv_sec) * 1000
			+ (current_time.tv_usec
				- sim->philos[i].last_meal_time.tv_usec) / 1000;
		if (sim->philos[i].meals_eaten != sim->meals_required
			&& elapsed_time > sim->time_to_die)
		{
			ft_print_message(sim, sim->philos[i].id, "died");
			pthread_mutex_unlock(&sim->philos[i].state_mutex);
			return (1);
		}
		pthread_mutex_unlock(&sim->philos[i].state_mutex);
	}
	return (0);
}

int	ft_check_all_philosophers_finished(t_sim *sim)
{
	int	all_finished;
	int	i;

	i = -1;
	all_finished = 1;
	while (++i < sim->num_philos)
	{
		pthread_mutex_lock(&sim->philos[i].state_mutex);
		if (sim->philos[i].finished_meals == 0)
			all_finished = 0;
		pthread_mutex_unlock(&sim->philos[i].state_mutex);
	}
	return (all_finished);
}

void	*ft_monitor_philosophers(void *arg)
{
	t_sim	*sim;
	int		all_finished;

	sim = (t_sim *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (ft_check_philosopher_deaths(sim))
		{
			sim->philo_died = 1;
			pthread_mutex_unlock(&sim->state_mutex);
			return (NULL);
		}
		all_finished = ft_check_all_philosophers_finished(sim);
		pthread_mutex_unlock(&sim->state_mutex);
		if (all_finished)
			break ;
		usleep(10);
	}
	return (NULL);
}
