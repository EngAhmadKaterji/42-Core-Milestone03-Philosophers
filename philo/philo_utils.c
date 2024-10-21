/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaterji <akaterji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:13:15 by akaterji          #+#    #+#             */
/*   Updated: 2024/08/22 17:15:46 by akaterji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_mutexes(t_sim *sim)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0
		|| pthread_mutex_init(&sim->state_mutex, NULL) != 0)
	{
		perror("Failed to initialize mutexes");
		return (1);
	}
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philos);
	if (sim->forks == NULL)
	{
		perror("Failed to allocate memory for forks");
		return (1);
	}
	while (++i < sim->num_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
		{
			perror("Failed to initialize fork mutex");
			return (1);
		}
	}
	return (0);
}

int	ft_init_philosophers(t_sim *sim, int i)
{
	while (++i < sim->num_philos)
	{
		sim->philos[i].id = i + 1;
		if (i % 2 == 0)
		{
			sim->philos[i].left_fork = &sim->forks[i];
			if (sim->num_philos > 1)
				sim->philos[i].right_fork
					= &sim->forks[(i + 1) % sim->num_philos];
		}
		else
		{
			sim->philos[i].left_fork = &sim->forks[(i + 1) % sim->num_philos];
			sim->philos[i].right_fork = &sim->forks[i];
		}
		sim->philos[i].sim = sim;
		gettimeofday(&sim->philos[i].last_meal_time, NULL);
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].finished_meals = 0;
		if (pthread_mutex_init(&sim->philos[i].state_mutex, NULL) != 0)
			return (perror("Failed to init pthread"), 1);
	}
	return (0);
}

void	ft_cleanup(t_sim *sim)
{
	int	i;

	i = -1;
	while (++i < sim->num_philos)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		pthread_mutex_destroy(&sim->philos[i].state_mutex);
	}
	free(sim->forks);
	free(sim->philos);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->state_mutex);
}

int	ft_create_threads(t_sim *sim, pthread_t *monitor_thread)
{
	int	i;

	i = -1;
	while (++i < sim->num_philos)
	{
		if (pthread_create(&(sim->philos[i].thread),
				NULL, ft_philosopher_life, &(sim->philos[i])) != 0)
		{
			perror("Failed to create philosopher thread");
			ft_cleanup(sim);
			return (1);
		}
	}
	if (pthread_create(monitor_thread, NULL, ft_monitor_philosophers, sim) != 0)
	{
		perror("Failed to create monitor thread");
		ft_cleanup(sim);
		return (1);
	}
	return (0);
}

int	ft_join_threads(t_sim *sim, pthread_t *monitor_thread)
{
	int	i;

	i = -1;
	while (++i < sim->num_philos)
	{
		if (pthread_join(sim->philos[i].thread, NULL) != 0)
		{
			perror("Failed to join philosopher thread");
			ft_cleanup(sim);
			return (1);
		}
	}
	if (pthread_join(*monitor_thread, NULL) != 0)
	{
		perror("Failed to join monitor thread");
		ft_cleanup(sim);
		return (1);
	}
	return (0);
}
