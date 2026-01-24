/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:05:18 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/24 15:59:23 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

long	get_time_ms(void)
{
	struct timeval	timeval;

	if (gettimeofday(&timeval, NULL))
		exit_with_error_msg("gettimeofday failed.");
	return ((timeval.tv_sec * 1e3) + (timeval.tv_usec / 1e3));
}

void	exit_with_error_msg(const char *msg)
{
	printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

void	safe_log_status(t_status status, t_philo *philo)
{
	long	time_passed;

	time_passed = get_time_ms() - safe_get_long(&philo->table->table_mutex,
			&philo->table->starting_time);
	if (philo->is_full)
		return ;
	safe_mutex_op(&philo->table->print_mutex, LOCK);
	if (!safe_get_bool(&philo->table->table_mutex,
			&philo->table->end_simulation) && (status == FORK_TAKEN
			|| status == RIGHT_FORK_TAKEN))
		printf("%-6ld %d has taken a fork\n", time_passed, philo->id);
	else if (status == EATING && !safe_get_bool(&philo->table->table_mutex,
			&philo->table->end_simulation))
		printf("%-6ld %d is eating\n", time_passed, philo->id);
	else if (status == SLEEPING && !safe_get_bool(&philo->table->table_mutex,
			&philo->table->end_simulation))
		printf("%-6ld %d is sleeping\n", time_passed, philo->id);
	else if (status == THINKING && !safe_get_bool(&philo->table->table_mutex,
			&philo->table->end_simulation))
		printf("%-6ld %d is thinking\n", time_passed, philo->id);
	else if (status == DIED)
		printf("%-6ld %d died\n", time_passed, philo->id);
	safe_mutex_op(&philo->table->print_mutex, UNLOCK);
}

void	clean(t_table *table)
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

void safe_usleep(long usec)
{
    long start = get_time_ms();
    long target = usec / 1000;

    while ((get_time_ms() - start) < target)
    {
        long remaining = target - (get_time_ms() - start);
        if (remaining > 1)
            usleep(remaining * 500);
    }
}
