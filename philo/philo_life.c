/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaterji <akaterji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:52:53 by akaterji          #+#    #+#             */
/*   Updated: 2024/08/26 14:36:46 by akaterji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	custom_usleep(int sleep_time_ms, t_philo *philo)
{
	int		slept_time;
	int		sleep_interval;
	bool	stop_sleep;
	t_sim	*sim;

	stop_sleep = false;
	slept_time = 0;
	sleep_interval = 10000;
	sim = philo->sim;
	while (slept_time < sleep_time_ms)
	{
		pthread_mutex_lock(&(sim->state_mutex));
		stop_sleep = sim->philo_died;
		pthread_mutex_unlock(&(sim->state_mutex));
		if (stop_sleep)
			break ;
		usleep(sleep_interval);
		slept_time += sleep_interval;
	}
}

void	ft_print_message(t_sim *sim, int id, const char *message)
{
	struct timeval	current_time;
	long			time_since_start;
	long			time_to_die;

	gettimeofday(&current_time, NULL);
	time_since_start = (current_time.tv_sec
			- sim->philos[id - 1].last_meal_time.tv_sec) * 1000
		+ (current_time.tv_usec
			- sim->philos[id - 1].last_meal_time.tv_usec) / 1000;
	time_to_die = sim->time_to_die - time_since_start;
	pthread_mutex_lock(&sim->print_mutex);
	printf("Philo %d %s | Time to Die: %ldms | Time Since Last Meal: %ldms\n",
		id, message, time_to_die, time_since_start);
	pthread_mutex_unlock(&sim->print_mutex);
}

void	ft_think_philo(t_philo *philo, t_sim *sim)
{
	long	thinking_time;

	thinking_time = sim->time_to_die - sim->time_to_eat - sim->time_to_sleep;
	pthread_mutex_lock(&sim->state_mutex);
	if (!sim->philo_died)
		ft_print_message(sim, philo->id, "is thinking");
	pthread_mutex_unlock(&sim->state_mutex);
	if (philo->id % 2 != 0 && thinking_time > 10)
		custom_usleep(sim->time_to_eat * 10, philo);
}

void	ft_finish_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->state_mutex);
	philo->finished_meals = 1;
	pthread_mutex_unlock(&philo->state_mutex);
}

void	*ft_philosopher_life(void *arg)
{
	t_philo	*philo;
	t_sim	*sim;
	int		iterations;

	philo = (t_philo *)arg;
	sim = philo->sim;
	iterations = 0;
	pthread_mutex_lock(&philo->state_mutex);
	gettimeofday(&philo->last_meal_time, NULL);
	pthread_mutex_unlock(&philo->state_mutex);
	ft_think_philo(philo, sim);
	while ((iterations < sim->meals_required
			|| sim->meals_required == -1) && !ft_check_philo_died(sim))
	{
		if (!ft_take_forks(philo, sim))
			return (NULL);
		if (ft_check_philo_died(sim))
			return (NULL);
		ft_eat_philo(philo, sim, &iterations);
		ft_sleep_philo(philo, sim);
		ft_think_philo(philo, sim);
	}
	return (ft_finish_meals(philo), NULL);
}
