/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 14:40:34 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 21:40:22 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	is_safe_atoi(char *arg)
{
	long	val;
	int		i;

	val = 0;
	i = 0;
	while (arg[i] != '\0')
	{
		val = (val * 10) + (arg[i] - '0');
		if (val > 2147483647)
			return (0);
		i++;
	}
	return (1);
}

static int	is_valid_argument(char *arg, int is_scheduler)
{
	int	i;

	i = 0;
	if (is_scheduler)
	{
		return (strcmp(arg, "fifo") == 0 || strcmp(arg, "edf") == 0);
	}
	if (arg[0] == '\0')
		return (0);
	while (arg[i] != '\0')
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (0);
		i++;
	}
	return (is_safe_atoi(arg));
}

static void	set_data_args(t_data *data, char **args)
{
	data->num_coders = atoi(args[1]);
	data->time_burnout = atoi(args[2]);
	data->time_compile = atoi(args[3]);
	data->time_debug = atoi(args[4]);
	data->time_refactor = atoi(args[5]);
	data->num_compiles = atoi(args[6]);
	data->dongle_cooldown = atoi(args[7]);
	data->scheduler = args[8];
}

t_data	*parser(char **args)
{
	t_data	*data;
	int		i;

	i = 1;
	while (i <= 7)
	{
		if (!is_valid_argument(args[i], 0))
			return (NULL);
		i++;
	}
	if (!is_valid_argument(args[8], 1))
		return (NULL);
	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	set_data_args(data, args);
	if (data->num_coders < 1)
		return (free(data), NULL);
	return (data);
}
