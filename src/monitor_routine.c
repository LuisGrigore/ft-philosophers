/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 16:05:05 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/28 16:05:18 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

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

void	*monitor_routine(void *table_ptr)
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
		usleep(500);
	}
	return (NULL);
}
