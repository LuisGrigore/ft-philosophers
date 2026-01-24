/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:07:08 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/24 03:20:46 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int argc, char const **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
		exit_with_error_msg("Wrong input!! Usage: ./philosophers <num> "
							"<time_to_die> <time_to_eat> <time_to_sleep> [meals_required]");
	parse_input(&table, argv);
	init_table(&table);
	start_simulation(&table);
	//clean(&table);
	return (0);
}
