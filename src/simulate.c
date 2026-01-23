/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:05:55 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 17:55:33 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <unistd.h>

void	wait_all_philos(t_table *table)
{
	while (!safe_get_bool(&table->table_mutex, &table->start_simulation))
		;
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
		usleep(table->time_to_sleep);
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
