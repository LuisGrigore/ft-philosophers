/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:05:55 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/24 03:39:43 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <unistd.h>

static void	eat(t_philo *philo)
{
	safe_mutex_op(&philo->right_fork->mutex, LOCK);
	safe_log_status(FORK_TAKEN, philo);
	safe_mutex_op(&philo->left_fork->mutex, LOCK);
	safe_log_status(FORK_TAKEN, philo);
	safe_set_long(&philo->philo_mutex, &philo->time_of_last_meal,
		get_time_ms());
	philo->meal_counter++;
	safe_log_status(EATING, philo);
	usleep(philo->table->time_to_eat);
	if (philo->table->meal_limit > 0
		&& philo->meal_counter == philo->table->meal_limit)
		safe_set_bool(&philo->philo_mutex, &philo->is_full, true);
	safe_mutex_op(&philo->right_fork->mutex, UNLOCK);
	safe_mutex_op(&philo->left_fork->mutex, UNLOCK);
}

void	*philo_start(void *philo_ptr)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)philo_ptr;
	table = philo->table;
	while (!safe_get_bool(&table->table_mutex, &table->start_simulation))
		;
	safe_set_long(&philo->philo_mutex, &philo->time_of_last_meal,
		get_time_ms());
	safe_mutex_op(&table->table_mutex, LOCK);
	table->n_philos_ready++;
	safe_mutex_op(&table->table_mutex, UNLOCK);
	while (!safe_get_bool(&table->table_mutex, &table->end_simulation))
	{
		if (philo->is_full)
			break ;
		eat(philo);
		safe_log_status(SLEEPING, philo);
		usleep(table->time_to_sleep);
		safe_log_status(THINKING, philo);
		// if (table->n_philos % 2 != 0 && table->time_to_eat * 2
		// 	- table->time_to_sleep > 0)
		// 	usleep((table->time_to_eat * 2 - table->time_to_sleep) * 0.70);
		//usleep(150);
	}
	return (NULL);
}

static bool	philo_died(t_philo *philo)
{
	long	time_passed;

	if (safe_get_bool(&philo->philo_mutex, &philo->is_full))
		return (false);
	time_passed = get_time_ms() - safe_get_long(&philo->philo_mutex,
			&philo->time_of_last_meal);
	if (time_passed > philo->table->time_to_die / 1e3)
		return (true);
	return (false);
}

void	*monitor_start(void *table_ptr)
{
	t_table	*table;
	int		i;

	table = (t_table *)table_ptr;
	while (!safe_get_bool(&table->table_mutex, &table->start_simulation)
		|| safe_get_long(&table->table_mutex,
			&table->n_philos_ready) < table->n_philos)
		;
	while (!safe_get_bool(&table->table_mutex, &table->end_simulation))
	{
		i = -1;
		while (++i < table->n_philos && !safe_get_bool(&table->table_mutex,
				&table->end_simulation))
		{
			if (philo_died(table->philos + i))
			{
				safe_set_bool(&table->table_mutex, &table->end_simulation,
					true);
				safe_log_status(DIED, table->philos + i);
			}
		}
	}
	return (NULL);
}

void	start_simulation(t_table *table)
{
	int	i;

	if (table->meal_limit == 0)
		return ;
	i = -1;
	while (++i < table->n_philos)
		safe_thread_op(&table->philos[i].thread_id, philo_start,
			&table->philos[i], CREATE);
	safe_thread_op(&table->monitor, monitor_start, table, CREATE);
	table->starting_time = get_time_ms();
	safe_set_bool(&table->table_mutex, &table->start_simulation, true);
	i = -1;
	while (++i < table->n_philos)
		safe_thread_op(&table->philos[i].thread_id, NULL, NULL, JOIN);
	safe_set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread_op(&table->monitor, NULL, NULL, JOIN);
}
