/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 09:51:59 by msbii             #+#    #+#             */
/*   Updated: 2026/09/04 11:55:18 by msbii            ###   ########.fr       */
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
	coders = init_coders(data);
	setup_env(data, coders);
	start_simulation(data);
	return (0);
}
