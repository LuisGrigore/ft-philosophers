/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_access.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 17:16:09 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 19:40:47 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	safe_set_bool(t_mutex *mutx, bool *target, bool value)
{
	safe_mutex_op(mutx, LOCK);
	*target = value;
	safe_mutex_op(mutx, UNLOCK);
}

bool	safe_get_bool(t_mutex *mutx, bool *target)
{
	bool	result;

	safe_mutex_op(mutx, LOCK);
	result = *target;
	safe_mutex_op(mutx, UNLOCK);
	return (result);
}

void	safe_set_long(t_mutex *mutx, long *target, long value)
{
	safe_mutex_op(mutx, LOCK);
	*target = value;
	safe_mutex_op(mutx, UNLOCK);
}

long	safe_get_long(t_mutex *mutx, long *target)
{
	long	result;

	safe_mutex_op(mutx, LOCK);
	result = *target;
	safe_mutex_op(mutx, UNLOCK);
	return (result);
}
