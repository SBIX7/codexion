/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_lifecycle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 19:54:12 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 11:25:45 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	sync_coder_request(t_data *data, t_coder *coder)
{
	long			wait_time;
	struct timespec	ts;

	if (strcmp(data->scheduler, "fifo") == 0)
		coder->request_order = data->request_count++;
	push_heap(data, coder);
	while (data->simulation_running && !can_take_dongles(data, coder))
	{
		wait_time = get_cooldown_remaining(data, coder);
		if (wait_time > 0)
			set_timewait(&ts, wait_time);
		else
			set_timewait(&ts, 5);
		pthread_cond_timedwait(&coder->cond, &data->simulation_mutex, &ts);
	}
}

void	*coder_wait_for_turn(t_data *data, t_coder *coder)
{
	pthread_mutex_lock(&data->simulation_mutex);
	sync_coder_request(data, coder);
	if (!data->simulation_running)
	{
		pthread_mutex_unlock(&data->simulation_mutex);
		return (NULL);
	}
	extract_from_heap(data, coder->coder_id);
	data->dongles_status[coder->coder_id - 1] = -1;
	data->dongles_status[coder->coder_id % data->num_coders] = -1;
	pthread_mutex_unlock(&data->simulation_mutex);
	return ((void *)coder);
}

void	coder_compile(t_data *data, t_coder *coder)
{
	int	idx1;
	int	idx2;
	int	first;
	int	second;

	idx1 = coder->coder_id - 1;
	idx2 = coder->coder_id % data->num_coders;
	order_dongles(idx1, idx2, &first, &second);
	pthread_mutex_lock(&data->dongles[first]);
	pthread_mutex_lock(&data->dongles[second]);
	pthread_mutex_lock(&coder->state_mutex);
	data->coders[idx1].last_compile_time = get_current_time();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	print_status(data, idx1 + 1, "has taken a dongle");
	print_status(data, idx1 + 1, "has taken a dongle");
	print_status(data, idx1 + 1, "is compiling");
	precise_usleep(data->time_compile, data);
}

void	coder_release(t_data *data, t_coder *coder)
{
	int		idx1;
	int		idx2;
	int		first;
	int		second;
	long	ready;
	int		i;

	idx1 = coder->coder_id - 1;
	idx2 = coder->coder_id % data->num_coders;
	order_dongles(idx1, idx2, &first, &second);
	pthread_mutex_unlock(&data->dongles[second]);
	pthread_mutex_unlock(&data->dongles[first]);
	pthread_mutex_lock(&data->simulation_mutex);
	ready = get_current_time() + data->dongle_cooldown;
	data->dongles_status[idx1] = ready;
	data->dongles_status[idx2] = ready;
	i = 0;
	while (i < data->queue_size)
	{
		pthread_cond_signal(&data->coders[data->queue[i] - 1].cond);
		i++;
	}
	pthread_mutex_unlock(&data->simulation_mutex);
}

void	coder_rest(t_data *data, t_coder *coder)
{
	int	id;

	id = coder->coder_id;
	print_status(data, id, "is debugging");
	precise_usleep(data->time_debug, data);
	print_status(data, id, "is refactoring");
	precise_usleep(data->time_refactor, data);
}
