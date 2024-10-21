/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaterji <akaterji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 17:46:55 by akaterji          #+#    #+#             */
/*   Updated: 2024/08/25 18:04:41 by akaterji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_sim {
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meals_required;
	int				philo_died;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	*forks;
	struct s_philo	*philos;
}	t_sim;

typedef struct s_philo {
	int				id;
	int				meals_eaten;
	int				finished_meals;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	state_mutex;
	pthread_t		thread;
	t_sim			*sim;
	struct timeval	last_meal_time;
}	t_philo;
void	*ft_monitor_philosophers(void *arg);
void	*ft_philosopher_life(void *arg);
int		ft_check_philo_died(t_sim *sim);
void	ft_print_message(t_sim *sim, int id, const char *message);
void	ft_think_philo(t_philo *philo, t_sim *sim);
void	ft_eat_philo(t_philo *philo, t_sim *sim, int *iterations);
void	ft_sleep_philo(t_philo *philo, t_sim *sim);
int		ft_take_forks(t_philo *philo, t_sim *sim);
int		ft_check_all_philosophers_finished(t_sim *sim);
void	ft_cleanup(t_sim *sim);
int		ft_init_philosophers(t_sim *sim, int i);
int		ft_create_threads(t_sim *sim, pthread_t *monitor_thread);
int		ft_init_mutexes(t_sim *sim);
int		ft_join_threads(t_sim *sim, pthread_t *monitor_thread);
void	custom_usleep(int sleep_time_ms, t_philo *philo);

#endif
