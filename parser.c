/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 14:40:34 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 11:01:36 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_valid_argument(char *arg, int schedular)
{
	int	i;

	i = 0;
	if (schedular)
	{
		return (strcmp(arg, "fifo") == 0 || strcmp(arg, "edf") == 0);
	}
	while (arg[i] != '\0')
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (0);
		i++;
	}
	if (!(atol(arg) <= 2147483647 && atol(arg) >= -2147483648))
		return (0);
	return (1);
}

t_data	*parser(char **args)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	if (is_valid_argument(args[1], 0))
		data->num_coders = atoi(args[1]);
	if (is_valid_argument(args[2], 0))
		data->time_burnout = atoi(args[2]);
	if (is_valid_argument(args[3], 0))
		data->time_compile = atoi(args[3]);
	if (is_valid_argument(args[4], 0))
		data->time_debug = atoi(args[4]);
	if (is_valid_argument(args[5], 0))
		data->time_refactor = atoi(args[5]);
	if (is_valid_argument(args[6], 0))
		data->num_compiles = atoi(args[6]);
	if (is_valid_argument(args[7], 0))
		data->dongle_cooldown = atoi(args[7]);
	if (is_valid_argument(args[8], 1))
		data->scheduler = args[8];
	return (data);
}
