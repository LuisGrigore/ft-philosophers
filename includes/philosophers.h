/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:25:59 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/28 16:10:23 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_table	t_table;

typedef struct s_fork
{
	int					id;
	t_mutex				mutex;
}						t_fork;

typedef struct s_philo
{
	int					id;
	long				meal_counter;
	long				time_of_last_meal;
	bool				is_full;
	t_fork				*left_fork;
	t_fork				*right_fork;
	pthread_t			thread_id;
	t_table				*table;
	t_mutex				philo_mutex;
}						t_philo;

struct					s_table
{
	long				n_philos;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				meal_limit;
	long				starting_time;
	t_philo				*philos;
	pthread_t			monitor;
	long				n_philos_ready;
	t_fork				*forks;
	bool				end_simulation;
	bool				start_simulation;
	t_mutex				table_mutex;
	t_mutex				print_mutex;
};

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	FORK_TAKEN,
	RIGHT_FORK_TAKEN,
	DIED,
}						t_status;

void					exit_with_error_msg(const char *msg);
long					get_time_ms(void);
void					safe_log_status(t_status status, t_philo *philo);
void					safe_usleep(long usec, t_table *table);

void					parse_input(t_table *table, const char **argv);

void					init_table(t_table *table);

void					simulate(t_table *table);

typedef enum e_mux_op
{
	LOCK,
	UNLOCK,
	FREE,
	INIT
}						t_mux_op;

typedef enum e_thread_op
{
	CREATE,
	JOIN,
	DETACH,
}						t_thread_op;

void					*safe_malloc(size_t size);
void					safe_mutex_op(t_mutex *mux, t_mux_op op);
void					safe_thread_op(pthread_t *thread,
							void *(*funct)(void *), void *data, t_thread_op op);

void					safe_set_bool(t_mutex *mutx, bool *target, bool value);
bool					safe_get_bool(t_mutex *mutx, bool *target);
void					safe_set_long(t_mutex *mutx, long *target, long value);
long					safe_get_long(t_mutex *mutx, long *target);

void					*many_philos_routine(void *philo_ptr);
void					*one_philo_routine(void *philo_ptr);
void					*monitor_routine(void *table_ptr);

#endif