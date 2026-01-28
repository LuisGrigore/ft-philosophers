/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:05:55 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/28 16:10:08 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <unistd.h>

static void	simulate_with_one_philo(t_table *table)
{
	safe_thread_op(&table->philos[0].thread_id, one_philo_routine,
		&table->philos[0], CREATE);
	safe_thread_op(&table->monitor, monitor_routine, table, CREATE);
	table->starting_time = get_time_ms();
	safe_set_bool(&table->table_mutex, &table->start_simulation, true);
	safe_thread_op(&table->philos[0].thread_id, NULL, NULL, JOIN);
	safe_set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread_op(&table->monitor, NULL, NULL, JOIN);
}

static void	simulate_with_many_philos(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
		safe_thread_op(&table->philos[i].thread_id, many_philos_routine,
			&table->philos[i], CREATE);
	safe_thread_op(&table->monitor, monitor_routine, table, CREATE);
	table->starting_time = get_time_ms();
	safe_set_bool(&table->table_mutex, &table->start_simulation, true);
	i = -1;
	while (++i < table->n_philos)
		safe_thread_op(&table->philos[i].thread_id, NULL, NULL, JOIN);
	safe_set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread_op(&table->monitor, NULL, NULL, JOIN);
}

void	simulate(t_table *table)
{
	if (table->meal_limit == 0)
		return ;
	if (table->n_philos == 1)
	{
		simulate_with_one_philo(table);
		return ;
	}
	simulate_with_many_philos(table);
}
