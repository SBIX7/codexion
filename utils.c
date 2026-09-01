/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 10:09:59 by msbii             #+#    #+#             */
/*   Updated: 2026/09/01 11:44:16 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	is_valid_argument(char *arg, int schedular)
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

t_data	*parser(char **args)
{
	int		i;
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

t_coder	*init_coders(t_data *data)
{
	t_coder	*coders;
	int		i;

	coders = malloc(sizeof(t_coder) * data->num_coders);
	if (coders == NULL)
		return (NULL);
	i = 0;
	while (i < data->num_coders)
	{
		coders[i].coder_id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_compile_time = 0;
		coders[i].data = data;
		i++;
	}
	return (coders);
}

static int	*init_mutexs(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->num_coders)
	{
		if (pthread_mutex_init(&data->dongles[i], NULL) != 0)
		{
			j = 0;
			while (j < i)
			{
				pthread_mutex_destroy(&data->dongles[j]);
				j++;
			}
			return (1);
		}
		i++;
	}
	if (!(pthread_mutex_init(&data->print_mutex, NULL)
			&& pthread_mutex_init(&data->queue_mutex, NULL)
			&& pthread_mutex_init(&data->simulation_mutex, NULL)))
		return (1);
	return (0);
}

int	setup_env(t_data *data, t_coder *coders)
{
	pthread_mutex_t	*dongles;
	int				*dongles_status;
	int				*queue;
	int				i;

	dongles = malloc(sizeof(pthread_mutex_t) * data->num_coders);
	dongles_status = malloc(sizeof(int) * data->num_coders);
	queue = malloc(sizeof(int) * data->num_coders);
	if (!(queue && dongles && dongles_status))
		return (1);
	data->coders = coders;
	if (init_mutexs(data) != 0)
		return (1);
	i = 0;
	while (i < data->num_coders)
	{
		data->dongles_status[i] = 0;
		data->queue[i] = 0;
		i++;
	}
	data->queue_size = 0;
	data->simulation_running = 1;
}
