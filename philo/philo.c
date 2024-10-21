/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaterji <akaterji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 09:55:02 by akaterji          #+#    #+#             */
/*   Updated: 2024/08/26 12:23:19 by akaterji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_check_argv(char *str)
{
	int	i;

	i = 0;
	while (str[++i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
	}
	return (0);
}

int	ft_integer_atoi(const char *ptr)
{
	unsigned long long int	result;
	int						i;

	i = 0;
	result = 0;
	while (ptr[i] != '\0' && ((ptr[i] > 8 && ptr[i] < 14) || ptr[i] == 32))
		i++;
	if (ptr[i] == '-' || ptr[i] == '+')
	{
		if (ptr[i] == '-')
			return (-1);
		i++;
	}
	while (ptr[i] != '\0' && ptr[i] >= '0' && ptr[i] <= '9')
	{
		result = result * 10 + ptr[i] - '0';
		i++;
	}
	if (result > INT_MAX)
		return (-1);
	return ((int)result);
}

int	ft_check(char *str)
{
	if (ft_check_argv(str) || ft_integer_atoi(str) <= 0)
		return (1);
	return (0);
}

int	ft_check_valid_argvs(char **str, t_sim *sim)
{
	if (ft_check_argv(str[1]) || ft_integer_atoi(str[1]) <= 0
		|| ft_integer_atoi(str[1]) >= 201)
		return (perror("invalid number of philosopher"), 1);
	else
		sim->num_philos = ft_integer_atoi(str[1]);
	if (ft_check(str[2]))
		return (perror("invalid time to die"), 1);
	else
		sim->time_to_die = ft_integer_atoi(str[2]);
	if (ft_check(str[3]))
		return (perror("invalid time to eat"), 1);
	else
		sim->time_to_eat = ft_integer_atoi(str[3]);
	if (ft_check(str[4]))
		return (perror("invalid time to sleep"), 1);
	else
		sim->time_to_sleep = ft_integer_atoi(str[4]);
	if (str[5] && (ft_check_argv(str[5]) || ft_integer_atoi(str[5]) <= 0))
		return (perror("invalid number of meals"), 1);
	else if (str[5])
		sim->meals_required = ft_integer_atoi(str[5]);
	else
		sim->meals_required = -1;
	sim->philo_died = 0;
	return (0);
}

int	main(int argc, char **argv)
{
	t_sim		sim;
	pthread_t	monitor_thread;

	if (argc == 5 || argc == 6)
	{
		if (ft_check_valid_argvs(argv, &sim))
			return (1);
		if (ft_init_mutexes(&sim) != 0)
			return (1);
		sim.philos = malloc(sizeof(t_philo) * sim.num_philos);
		if (sim.philos == NULL)
			return (perror("Failed to malloc"), 1);
		if (ft_init_philosophers(&sim, -1) != 0)
			return (1);
		if (ft_create_threads(&sim, &monitor_thread) != 0)
			return (1);
		if (ft_join_threads(&sim, &monitor_thread) != 0)
			return (1);
		ft_cleanup(&sim);
	}
	else
		perror("incorrect number of arguments");
	return (0);
}
