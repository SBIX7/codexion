/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 11:49:41 by msbii             #+#    #+#             */
/*   Updated: 2026/09/04 13:23:24 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	start_simulation(t_data *data)
{
	int			i;
	void		*arg;
	pthread_t	monitor_thread;

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
