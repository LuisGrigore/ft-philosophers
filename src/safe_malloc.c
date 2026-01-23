/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:06:32 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/23 19:41:01 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <stdlib.h>

void	*safe_malloc(size_t size)
{
	void	*space;

	space = malloc(size);
	if (!space)
		exit_with_error_msg("Malloc failed.");
	return (space);
}
