/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 22:19:32 by msbii             #+#    #+#             */
/*   Updated: 2026/09/04 22:42:20 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	precise_usleep(long time_in_ms, t_data *data)
{
	long	start_time;

	start_time = get_current_time();
	while (get_current_time() - start_time < time_in_ms
		&& data->simulation_running)
	{
		usleep(100);
	}
}

void	handle_coder_burnout(t_data *data, int i)
{
	pthread_mutex_lock(&data->simulation_mutex);
	data->simulation_running = 0;
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d burned out\n", get_current_time() - data->start_time,
		data->coders[i].coder_id);
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_unlock(&data->simulation_mutex);
	pthread_mutex_unlock(&data->coders[i].state_mutex);
	i = 0;
	while (i < data->num_coders)
	{
		pthread_cond_signal(&data->coders[i].cond);
		i++;
	}
}

void	shutdown_simulation(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->simulation_mutex);
	data->simulation_running = 0;
	pthread_mutex_unlock(&data->simulation_mutex);
	i = 0;
	while (i < data->num_coders)
	{
		pthread_cond_signal(&data->coders[i].cond);
		i++;
	}
}

int	check_coders_status(t_data *data)
{
	int		i;
	long	last_compile;
	int		finished_coders;

	finished_coders = 0;
	i = 0;
	while (++i <= data->num_coders)
	{
		pthread_mutex_lock(&data->coders[i - 1].state_mutex);
		last_compile = data->coders[i - 1].last_compile_time;
		if (get_current_time() - last_compile > data->time_burnout)
		{
			handle_coder_burnout(data, i - 1);
			return (1);
		}
		if (data->coders[i - 1].compile_count >= data->num_compiles)
			finished_coders += 1;
		pthread_mutex_unlock(&data->coders[i - 1].state_mutex);
	}
	if (finished_coders == data->num_coders)
	{
		shutdown_simulation(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (data->simulation_running)
	{
		if (check_coders_status(data) == 1)
			break ;
		usleep(1000);
	}
	return (NULL);
}
