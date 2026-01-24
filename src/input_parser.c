/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrigore <lgrigore@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:15:12 by lgrigore          #+#    #+#             */
/*   Updated: 2026/01/24 02:55:18 by lgrigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"
#include <limits.h>

static long	ft_atol(const char *str)
{
	long	x;
	int		sign;
	long	i;

	x = 0;
	sign = 1;
	i = 0;
	if (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t' || (str[i] >= 9 && str[i] <= 13))
			i++;
		if (str[i] == '+' && (str[i + 1] >= '0' && str[i + 1] <= '9'))
			i++;
		if (str[i] == '-' && (str[i + 1] >= '0' && str[i + 1] <= '9'))
		{
			sign = -1;
			i++;
		}
		while (str[i] >= '0' && str[i] <= '9')
		{
			x = x * 10 + (str[i] - '0');
			i++;
		}
	}
	return (sign * x);
}

static int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
	{
		return (1);
	}
	return (0);
}

static int	ft_isdigit_str(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]))
			return (0);
		i++;
	}
	return (1);
}

static long	parse_time(const char *str)
{
	long	time;

	if (!ft_isdigit_str(str))
		exit_with_error_msg("Arguments must be numeric.");
	time = ft_atol(str);
	if (time < 60 || time > INT_MAX)
		exit_with_error_msg("<time_to_die> <time_to_eat> <time_to_sleep> "
							"should be numbers between 60 and INT_MAX.");
	return (time);
}

void	parse_input(t_table *table, const char **argv)
{
	if (!ft_isdigit_str(argv[1]))
		exit_with_error_msg("<num> must be a positive number.");
	table->n_philos = ft_atol(argv[1]);
	if (table->n_philos < 1 || table->n_philos > 200)
		exit_with_error_msg("<num> should be a number between 1 and 200.");
	table->time_to_die = parse_time(argv[2]) * 1000L;
	table->time_to_eat = parse_time(argv[3]) * 1000L;
	table->time_to_sleep = parse_time(argv[4]) * 1000L;
	if (argv[5])
	{
		if (!ft_isdigit_str(argv[5]))
			exit_with_error_msg("<meal_limit> must be a positive number.");
		table->meal_limit = ft_atol(argv[5]);
		if (table->meal_limit <= 0)
			exit_with_error_msg("<meal_limit> must be > 0.");
	}
	else
		table->meal_limit = -1;
}
