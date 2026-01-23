/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:05:18 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 17:37:24 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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