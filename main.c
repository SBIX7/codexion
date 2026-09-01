/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 09:51:59 by msbii             #+#    #+#             */
/*   Updated: 2026/09/01 10:46:05 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	main(int argc, char **argv)
{
	int		i;
	t_data	*data;
	t_coder *coders;

	if (argc != 9)
	{
		printf("Error: There is eight mandatory options to implicitly define.\n");
		return (1);
	}
	data = parser(argv);
	coders = init_coders(data);
	setup_env()
	return (0);
}
