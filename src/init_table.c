/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:59:35 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 16:58:16 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	init_philos(t_table *table)
{
	int		i;
	t_philo	*current_philo;

	i = -1;
	while (++i < table->number_of_philosophers)
	{
		current_philo = table->philos + i;
		current_philo->id = i + 1;
		current_philo->is_full = false;
		current_philo->meal_counter = 0;
		current_philo->table = table;
		if (current_philo->id % 2)
		{
			current_philo->right_fork = &table->forks[i];
			current_philo->left_fork = &table->forks[(i + 1)
				% table->number_of_philosophers];
		}
		else
		{
			current_philo->left_fork = &table->forks[i];
			current_philo->right_fork = &table->forks[(i + 1)
				% table->number_of_philosophers];
		}
	}
}

static void	init_forks(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->number_of_philosophers)
	{
		safe_mutex_op(&table->forks[i].mutex, INIT);
		table->forks[i].id = i;
	}
}

void	init_table(t_table *table)
{
	table->end_simulation = false;
	table->start_simulation = false;
	table->philos = safe_malloc(sizeof(t_philo)
			* table->number_of_philosophers);
	table->forks = safe_malloc(sizeof(t_fork) * table->number_of_philosophers);
	init_forks(table);
	init_philos(table);
	safe_mutex_op(&table->mutex, INIT);
}