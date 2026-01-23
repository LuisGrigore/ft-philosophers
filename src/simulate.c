/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:05:55 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 19:31:08 by lgrigore         ###   ########.fr       */
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
	while (!safe_get_bool(&table->table_mutex, &table->end_simulation))
	{
		if (philo->is_full)
			break ;
		eat(philo);
		safe_log_status(SLEEPING, philo);
		usleep(table->time_to_sleep);
		safe_log_status(THINKING, philo);
	}
	return (NULL);
}

void	start_simulation(t_table *table)
{
	int	i;

	if (table->meal_limit == 0)
		return ;
	if (table->number_of_philosophers == 1)
		// TODO
		return ;
	i = -1;
	while (++i < table->number_of_philosophers)
		safe_thread_op(&table->philos[i].thread_id, philo_start,
			&table->philos[i], CREATE);
	table->starting_time = get_time_ms();
	safe_set_bool(&table->table_mutex, &table->start_simulation, true);
	i = -1;
	while (++i < table->number_of_philosophers)
		safe_thread_op(&table->philos[i].thread_id, NULL, NULL, JOIN);
}
