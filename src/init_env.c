/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 22:04:00 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 22:26:15 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	init_mutexs(t_data *data)
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
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0
		|| pthread_mutex_init(&data->simulation_mutex, NULL) != 0)
		return (1);
	return (0);
}

static void	init_simulation_state(t_data *data)
{
	int	i;

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

static int	alloc_env(t_data *data)
{
	data->dongles = malloc(sizeof(pthread_mutex_t) * data->num_coders);
	data->dongles_status = malloc(sizeof(long) * data->num_coders);
	data->queue = malloc(sizeof(int) * data->num_coders);
	if (!(data->queue && data->dongles && data->dongles_status))
	{
		free(data->dongles);
		free(data->dongles_status);
		free(data->queue);
		return (1);
	}
	return (0);
}

int	setup_env(t_data *data, t_coder *coders)
{
	int	i;

	if (alloc_env(data) != 0)
		return (1);
	data->coders = coders;
	data->request_count = 0;
	data->start_time = get_current_time();
	i = -1;
	while (++i < data->num_coders)
		data->coders[i].last_compile_time = data->start_time;
	if (init_mutexs(data) != 0)
		return (1);
	init_simulation_state(data);
	return (0);
}
