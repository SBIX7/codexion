/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 10:09:59 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 22:05:00 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

static void	cleanup_coders(t_coder *coders, int count)
{
	int	j;

	j = 0;
	while (j < count)
		pthread_mutex_destroy(&coders[j++].state_mutex);
	free(coders);
}

t_coder	*init_coders(t_data *data)
{
	t_coder	*coders;
	int		i;

	coders = malloc(sizeof(t_coder) * data->num_coders);
	if (coders == NULL)
		return (NULL);
	i = -1;
	while (++i < data->num_coders)
	{
		coders[i].coder_id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_compile_time = 0;
		coders[i].request_order = 0;
		coders[i].data = data;
		if (pthread_mutex_init(&coders[i].state_mutex, NULL) != 0)
		{
			cleanup_coders(coders, i);
			return (NULL);
		}
	}
	return (coders);
}
