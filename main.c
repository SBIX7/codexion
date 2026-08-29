/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 09:51:59 by msbii             #+#    #+#             */
/*   Updated: 2026/08/29 10:11:07 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	main(int argc, char **argv)
{
	int		i;
	t_info	*infos;

	if (argc != 9)
	{
		printf("Error: There is eight mandatory options to implicitly define.\n");
		return (1);
	}
	infos = parser(argv);
	
	return (0);
}
