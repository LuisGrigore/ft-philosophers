/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:59:35 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/24 01:41:45 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	init_philos(t_table *table)
{
	int		i;
	t_philo	*current_philo;

	i = -1;
	while (++i < table->n_philos)
	{
		current_philo = table->philos + i;
		current_philo->id = i + 1;
		current_philo->is_full = false;
		current_philo->meal_counter = 0;
		current_philo->table = table;
		safe_mutex_op(&current_philo->philo_mutex, INIT);
		if (current_philo->id % 2 == 0)
		{
			current_philo->right_fork = &table->forks[i];
			current_philo->left_fork = &table->forks[(i + 1) % table->n_philos];
		}
		else
		{
			current_philo->left_fork = &table->forks[i];
			current_philo->right_fork = &table->forks[(i + 1)
				% table->n_philos];
		}
	}
}

static void	init_forks(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
	{
		safe_mutex_op(&table->forks[i].mutex, INIT);
		table->forks[i].id = i;
	}
}

void	init_table(t_table *table)
{
	table->end_simulation = false;
	table->start_simulation = false;
	table->philos = safe_malloc(sizeof(t_philo) * table->n_philos);
	table->forks = safe_malloc(sizeof(t_fork) * table->n_philos);
	init_forks(table);
	init_philos(table);
	safe_mutex_op(&table->table_mutex, INIT);
	safe_mutex_op(&table->print_mutex, INIT);
	table->n_philos_ready = 0;
}
