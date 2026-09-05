/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 09:51:59 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 14:01:21 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	main(int argc, char **argv)
{
	t_data	*data;
	t_coder	*coders;

	if (argc != 9)
	{
		printf("Error: There is eight mandatory options to define.\n");
		return (1);
	}
	data = parser(argv);
	if (!data)
	{
		printf("Error: Invalid arguments.\n");
		return (1);
	}
	coders = init_coders(data);
	if (!coders || setup_env(data, coders) != 0)
	{
		printf("Error: Initialization failed.\n");
		if (coders)
			free(coders);
		free(data);
		return (1);
	}
	start_simulation(data);
	clean_exit(data);
	return (0);
}
