/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:02:45 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 21:33:50 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	swap(int *queue, int a, int b)
{
	int	c;

	c = queue[a];
	queue[a] = queue[b];
	queue[b] = c;
}

static void	shift_down(t_data *data, int i)
{
	int	left;
	int	right;
	int	urgent_child;

	while (((i * 2) + 1) < data->queue_size)
	{
		left = (i * 2) + 1;
		right = (i * 2) + 2;
		if (right < data->queue_size && is_priority(data, data->queue[right],
				data->queue[left]))
			urgent_child = right;
		else
			urgent_child = left;
		if (is_priority(data, data->queue[urgent_child], data->queue[i]))
		{
			swap(data->queue, urgent_child, i);
			i = urgent_child;
		}
		else
			break ;
	}
}

int	is_priority(t_data *data, int id_1, int id_2)
{
	t_coder	*coder1;
	t_coder	*coder2;

	coder1 = &data->coders[id_1 - 1];
	coder2 = &data->coders[id_2 - 1];
	if (strcmp("edf", data->scheduler) == 0)
	{
		if (coder1->last_compile_time < coder2->last_compile_time)
			return (1);
		if (coder1->last_compile_time == coder2->last_compile_time)
		{
			if (coder1->request_order < coder2->request_order)
				return (1);
		}
	}
	if (strcmp("fifo", data->scheduler) == 0)
		if (coder1->request_order < coder2->request_order)
			return (1);
	return (0);
}

void	push_heap(t_data *data, t_coder *coder)
{
	int	i;
	int	parent_index;

	i = data->queue_size;
	data->queue[i] = coder->coder_id;
	while (i > 0)
	{
		parent_index = (i - 1) / 2;
		if (is_priority(data, coder->coder_id, data->queue[parent_index]))
			swap(data->queue, i, parent_index);
		else
			break ;
		i = (i - 1) / 2;
	}
	data->queue_size += 1;
}

void	extract_from_heap(t_data *data, int coder_id)
{
	int	i;

	i = 0;
	while (i < data->queue_size)
	{
		if (data->queue[i] == coder_id)
			break ;
		i++;
	}
	if (data->queue_size == i)
		return ;
	data->queue[i] = data->queue[data->queue_size - 1];
	data->queue_size -= 1;
	shift_down(data, i);
}
