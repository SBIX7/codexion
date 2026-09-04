/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 10:09:59 by msbii             #+#    #+#             */
/*   Updated: 2026/09/04 12:03:23 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
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
		if (pthread_cond_init(&coders[i].cond, NULL) != 0)
			return (NULL);
		i++;
	}
	return (coders);
}

int	init_mutexs(t_data *data)
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
		|| pthread_mutex_init(&data->queue_mutex, NULL) != 0
		|| pthread_mutex_init(&data->simulation_mutex, NULL) != 0)
		return (1);
	return (0);
}

void	init_simulation_state(t_data *data)
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

int	setup_env(t_data *data, t_coder *coders)
{
	pthread_mutex_t	*dongles;
	long			*dongles_status;
	int				*queue;
	int				i;

	dongles = malloc(sizeof(pthread_mutex_t) * data->num_coders);
	dongles_status = malloc(sizeof(long) * data->num_coders);
	queue = malloc(sizeof(int) * data->num_coders);
	if (!(queue && dongles && dongles_status))
		return (1);
	data->coders = coders;
	data->dongles = dongles;
	data->dongles_status = dongles_status;
	data->queue = queue;
	data->request_count = 0;
	data->start_time = get_current_time();
	i = 0;
	while (++i <= data->num_coders)
		data->coders[i - 1].last_compile_time = data->start_time;
	if (init_mutexs(data) != 0)
		return (1);
	init_simulation_state(data);
	return (0);
}
