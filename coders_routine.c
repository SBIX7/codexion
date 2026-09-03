/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:36:00 by msbii             #+#    #+#             */
/*   Updated: 2026/09/03 22:22:18 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	dongles_free(t_coder *coder, t_data *data)
{
	int		dongle_i;
	int		n_coders;
	long	*dongles_status;

	n_coders = data->num_coders;
	if (data->num_coders == 1)
		return (0);
	dongles_status = data->dongles_status;
	dongle_i = coder->coder_id - 1;
	if ((dongles_status[dongle_i] != -1
			&& get_current_time() >= dongles_status[dongle_i])
		&& (dongles_status[(dongle_i + 1) % n_coders] != -1
			&& get_current_time() >= dongles_status[(dongle_i + 1) % n_coders]))
		return (1);
	return (0);
}

long	get_cooldown_remaining(t_data *data, t_coder *coder)
{
	int		idx1;
	int		idx2;
	long	current;
	long	wait;

	idx1 = coder->coder_id - 1;
	idx2 = coder->coder_id % data->num_coders;
	current = get_current_time();
	wait = 0;
	if (data->dongles_status[idx1] != -1
		&& data->dongles_status[idx1] > current)
		wait = data->dongles_status[idx1] - current;
	if (data->dongles_status[idx2] != -1
		&& data->dongles_status[idx2] > current)
		if (data->dongles_status[idx2] - current > wait)
			wait = data->dongles_status[idx2] - current;
	return (wait);
}

void	take_dongles(t_data *data, t_coder *coder)
{
	pop_heap(data);
	data->dongles_status[coder->coder_id - 1] = -1;
	data->dongles_status[coder->coder_id % data->num_coders] = -1;
}

void	set_timewait(struct timespec *ts, long wait_time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ts->tv_sec = tv.tv_sec + (wait_time / 1000);
	ts->tv_nsec = (tv.tv_usec * 1000) + ((wait_time % 1000) * 1000000);
	if (ts->tv_nsec >= 1000000000)
	{
		ts->tv_sec += 1;
		ts->tv_nsec -= 1000000000;
	}
}

void	*routine(void *arg)
{
	t_coder	*c_arg;
	t_data	*sim_data;

	c_arg = (t_coder *)arg;
	sim_data = c_arg->data;
	while (sim_data->simulation_running)
	{
		if (coder_wait_for_turn(sim_data, c_arg) == NULL)
			return (NULL);
		coder_compile(sim_data, c_arg);
		coder_release(sim_data, c_arg);
		coder_rest(sim_data, c_arg);
	}
	return (NULL);
}
