/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 11:49:41 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 21:33:50 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	clean_exit(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_destroy(&data->coders[i].state_mutex);
		pthread_mutex_destroy(&data->dongles[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->simulation_mutex);
	free(data->dongles);
	free(data->dongles_status);
	free(data->queue);
	free(data->coders);
	free(data);
}

void	order_dongles(int idx1, int idx2, int *first, int *second)
{
	if (idx1 < idx2)
	{
		*first = idx1;
		*second = idx2;
	}
	else
	{
		*first = idx2;
		*second = idx1;
	}
}

int	get_sim_status(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->simulation_mutex);
	status = data->simulation_running;
	pthread_mutex_unlock(&data->simulation_mutex);
	return (status);
}

void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->simulation_mutex);
	if (data->simulation_running)
	{
		printf("%ld %d %s\n", get_current_time() - data->start_time, id,
			status);
	}
	pthread_mutex_unlock(&data->simulation_mutex);
	pthread_mutex_unlock(&data->print_mutex);
}

void	start_simulation(t_data *data)
{
	int			i;
	void		*arg;
	pthread_t	monitor_thread;

	if (data->num_compiles == 0)
		return ;
	i = 0;
	while (i < data->num_coders)
	{
		arg = (void *)(data->coders + i);
		pthread_create(&data->coders[i].thread, NULL, &routine, arg);
		i++;
	}
	arg = (void *)data;
	pthread_create(&monitor_thread, NULL, &monitor_routine, arg);
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data->num_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
}
