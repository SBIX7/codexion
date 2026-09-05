/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:36:00 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 12:26:34 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	dongles_free(t_coder *coder, t_data *data)
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

int	can_take_dongles(t_data *data, t_coder *coder)
{
	int	o_id;
	int	i;

	if (!dongles_free(coder, data))
		return (0);
	i = 0;
	while (i < data->queue_size)
	{
		o_id = data->queue[i];
		if (o_id != coder->coder_id)
		{
			if ((o_id - 1 == coder->coder_id % data->num_coders || o_id
					% data->num_coders == coder->coder_id - 1)
				&& is_priority(data, o_id, coder->coder_id))
			{
				if (data->dongles_status[o_id - 1] != -1
					&& data->dongles_status[o_id % data->num_coders] != -1)
					return (0);
			}
		}
		i++;
	}
	return (1);
}

void	*routine(void *arg)
{
	t_coder	*c_arg;
	t_data	*sim_data;

	c_arg = (t_coder *)arg;
	sim_data = c_arg->data;
	while (get_sim_status(sim_data))
	{
		if (coder_wait_for_turn(sim_data, c_arg) == NULL)
			return (NULL);
		coder_compile(sim_data, c_arg);
		coder_release(sim_data, c_arg);
		coder_rest(sim_data, c_arg);
	}
	return (NULL);
}
