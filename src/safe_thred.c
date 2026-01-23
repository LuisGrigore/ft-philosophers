/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_thred.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:39:00 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 19:40:33 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	handle_thread_return(int return_code, t_thread_op op)
{
	if (return_code == 0)
		return ;
	if (op == JOIN)
		exit_with_error_msg("Thread join failed.");
	else if (op == CREATE)
		exit_with_error_msg("Thread create failed.");
	else if (op == DETACH)
		exit_with_error_msg("Thread detach failed.");
}

void	safe_thread_op(pthread_t *thread, void *(*funct)(void *), void *data,
		t_thread_op op)
{
	if (op == JOIN)
		handle_thread_return(pthread_join(*thread, NULL), op);
	else if (op == CREATE)
		handle_thread_return(pthread_create(thread, NULL, funct, data), op);
	else if (op == DETACH)
		handle_thread_return(pthread_detach(*thread), op);
}
