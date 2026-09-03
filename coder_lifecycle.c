/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_lifecycle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 19:54:12 by msbii             #+#    #+#             */
/*   Updated: 2026/09/03 22:21:21 by msbii            ###   ########.fr       */
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
	while (data->simulation_running && (data->queue[0] != coder->coder_id
			|| !dongles_free(coder, data)))
	{
		if (data->queue[0] == coder->coder_id)
		{
			wait_time = get_cooldown_remaining(data, coder);
			if (wait_time > 0)
			{
				set_timewait(&ts, wait_time);
				pthread_cond_timedwait(&coder->cond, &data->simulation_mutex,
					&ts);
				continue ;
			}
		}
		pthread_cond_wait(&coder->cond, &data->simulation_mutex);
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
	take_dongles(data, coder);
	pthread_mutex_unlock(&data->simulation_mutex);
	return ((void *)coder);
}

static void	coder_compile(t_data *data, t_coder *coder)
{
	int	idx1;
	int	idx2;

	idx1 = coder->coder_id - 1;
	idx2 = coder->coder_id % data->num_coders;
	pthread_mutex_lock(&data->dongles[idx1]);
	pthread_mutex_lock(&data->dongles[idx2]);
	pthread_mutex_lock(&coder->state_mutex);
	data->coders[idx1].last_compile_time = get_current_time();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d has taken a dongle\n", get_current_time() - data->start_time,
		idx1 + 1);
	printf("%ld %d has taken a dongle\n", get_current_time() - data->start_time,
		idx1 + 1);
	printf("%ld %d is compiling\n", get_current_time() - data->start_time, idx1
		+ 1);
	pthread_mutex_unlock(&data->print_mutex);
	usleep(data->time_compile * 1000);
}

void	coder_release(t_data *data, t_coder *coder)
{
	int		idx1;
	int		idx2;
	long	ready;

	idx1 = coder->coder_id - 1;
	idx2 = coder->coder_id % data->num_coders;
	pthread_mutex_unlock(&data->dongles[idx1]);
	pthread_mutex_unlock(&data->dongles[idx2]);
	pthread_mutex_lock(&data->simulation_mutex);
	ready = get_current_time() + data->dongle_cooldown;
	data->dongles_status[idx1] = ready;
	data->dongles_status[idx2] = ready;
	if (data->queue_size > 0)
		pthread_cond_signal(&data->coders[data->queue[0] - 1].cond);
	pthread_mutex_unlock(&data->simulation_mutex);
}

void	coder_rest(t_data *data, t_coder *coder)
{
	int	id;

	id = coder->coder_id;
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d is debugging\n", get_current_time() - data->start_time, id);
	pthread_mutex_unlock(&data->print_mutex);
	usleep(data->time_debug * 1000);
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d is refactoring\n", get_current_time() - data->start_time,
		id);
	pthread_mutex_unlock(&data->print_mutex);
	usleep(data->time_refactor * 1000);
}
