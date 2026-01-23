/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 15:05:55 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 17:40:01 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	wait_all_philos(t_table *table)
{
	while (!safe_get_bool(&table->mutex, table->start_simulation))
		;
}

void	*philo_start(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	while (!safe_get_bool(&philo->table->mutex, philo->table->start_simulation))
		;
	return (NULL);
}

void	start_simulation(t_table *table)
{
	int i;

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
	safe_set_bool(&table->mutex, &table->start_simulation, true);
}