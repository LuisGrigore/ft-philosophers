/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:03:31 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/28 16:05:22 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

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
	safe_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meal_limit > 0
		&& philo->meal_counter == philo->table->meal_limit)
		safe_set_bool(&philo->philo_mutex, &philo->is_full, true);
	safe_mutex_op(&philo->right_fork->mutex, UNLOCK);
	safe_mutex_op(&philo->left_fork->mutex, UNLOCK);
}

void	*many_philos_routine(void *philo_ptr)
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
		safe_usleep(table->time_to_sleep, table);
		safe_log_status(THINKING, philo);
		usleep(500);
	}
	return (NULL);
}

void	*one_philo_routine(void *philo_ptr)
{
	t_philo	*philo;
	t_table	*table;
	long	time_passed;

	philo = (t_philo *)philo_ptr;
	table = philo->table;
	while (!safe_get_bool(&table->table_mutex, &table->start_simulation))
		usleep(100);
	safe_set_long(&philo->philo_mutex, &philo->time_of_last_meal,
		get_time_ms());
	safe_mutex_op(&table->table_mutex, LOCK);
	table->n_philos_ready++;
	safe_mutex_op(&table->table_mutex, UNLOCK);
	safe_mutex_op(&philo->left_fork->mutex, LOCK);
	safe_log_status(FORK_TAKEN, philo);
	while (!safe_get_bool(&table->table_mutex, &table->end_simulation))
	{
		time_passed = get_time_ms() - safe_get_long(&philo->philo_mutex,
				&philo->time_of_last_meal);
		if (time_passed >= table->time_to_die)
			break ;
		usleep(100);
	}
	return (NULL);
}
