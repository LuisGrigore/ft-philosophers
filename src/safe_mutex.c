/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:11:47 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 17:15:37 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	handle_mutex_return(int return_code, t_mux_op op)
{
	if (return_code == 0)
		return ;
	if (op == LOCK)
		exit_with_error_msg("Mutex lock op failed.");
	else if (op == UNLOCK)
		exit_with_error_msg("Mutex unlock op failed.");
	else if (op == FREE)
		exit_with_error_msg("Mutex free op failed.");
	else if (op == INIT)
		exit_with_error_msg("Mutex alloc op failed.");
}

void	safe_mutex_op(t_mutex *mux, t_mux_op op)
{
	if (op == LOCK)
		handle_mutex_return(pthread_mutex_lock(mux), op);
	else if (op == UNLOCK)
		handle_mutex_return(pthread_mutex_unlock(mux), op);
	else if (op == FREE)
		handle_mutex_return(pthread_mutex_destroy(mux), op);
	else if (op == INIT)
		handle_mutex_return(pthread_mutex_init(mux, NULL), op);
}