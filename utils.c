/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 10:09:59 by msbii             #+#    #+#             */
/*   Updated: 2026/08/29 10:14:04 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_valid_argument(char *arg, int schedular)
{
	int	i;

	i = 0;
	if (schedular)
	{
		return ((strcmp(arg, "fifo") || strcmp(arg, "edf")) == 0);
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

t_info	*parser(char **args)
{
	int		i;
	t_info	*infos;

	infos = malloc(sizeof(t_info));
	if (infos == NULL)
		return (NULL);
	if (is_valid_argument(args[1], 0))
		infos->num_coders = atoi(args[1]);
	if (is_valid_argument(args[2], 0))
		infos->time_burnout = atoi(args[2]);
	if (is_valid_argument(args[3], 0))
		infos->time_compile = atoi(args[3]);
	if (is_valid_argument(args[4], 0))
		infos->time_debug = atoi(args[4]);
	if (is_valid_argument(args[5], 0))
		infos->time_refactor = atoi(args[5]);
	if (is_valid_argument(args[6], 0))
		infos->num_compiles = atoi(args[6]);
	if (is_valid_argument(args[7], 0))
		infos->dongle_cooldown = atoi(args[7]);
	if (is_valid_argument(args[8], 1))
		infos->scheduler = args[8];
	return (infos);
}
