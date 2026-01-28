/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:07:08 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/28 16:11:17 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	clean(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
		safe_mutex_op(&(table->philos + i)->philo_mutex, FREE);
	safe_mutex_op(&table->table_mutex, FREE);
	safe_mutex_op(&table->print_mutex, FREE);
	free(table->forks);
	free(table->philos);
}

int	main(int argc, char const **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
		exit_with_error_msg("Wrong input!! Usage: ./philosophers <num> "
			"<time_to_die> <time_to_eat> <time_to_sleep> [meals_required]");
	parse_input(&table, argv);
	init_table(&table);
	simulate(&table);
	clean(&table);
	return (0);
}
