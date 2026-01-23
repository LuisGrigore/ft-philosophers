/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 16:25:59 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 17:55:55 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>

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
}						t_philo;

struct					s_table
{
	long				number_of_philosophers;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				meal_limit;
	long				starting_time;
	t_philo				*philos;
	t_fork				*forks;
	bool				end_simulation;
	bool				start_simulation;
	t_mutex				table_mutex;
	t_mutex				print_mutex;
};

void					exit_with_error_msg(const char *msg);
long					get_time_ms(void);

void					parse_input(t_table *table, const char **argv);

void					init_table(t_table *table);

void					start_simulation(t_table *table);

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

#endif