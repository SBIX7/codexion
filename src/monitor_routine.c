/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 22:19:32 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 21:33:50 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	precise_usleep(long time_in_ms, t_data *data)
{
	long	start_time;

	start_time = get_current_time();
	while (get_current_time() - start_time < time_in_ms && get_sim_status(data))
		usleep(100);
}

static void	handle_coder_burnout(t_data *data, int i)
{
	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->simulation_mutex);
	if (data->simulation_running)
	{
		data->simulation_running = 0;
		printf("%ld %d burned out\n", get_current_time() - data->start_time,
			data->coders[i].coder_id);
	}
	pthread_mutex_unlock(&data->simulation_mutex);
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_unlock(&data->coders[i].state_mutex);
}

static void	shutdown_simulation(t_data *data)
{
	pthread_mutex_lock(&data->simulation_mutex);
	data->simulation_running = 0;
	pthread_mutex_unlock(&data->simulation_mutex);
}

static int	check_coders_status(t_data *data)
{
	long	last_compile;
	int		finished_coders;
	int		i;

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
	while (get_sim_status(data))
	{
		if (check_coders_status(data) == 1)
			break ;
		usleep(1000);
	}
	return (NULL);
}
