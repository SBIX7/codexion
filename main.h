/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 09:51:30 by msbii             #+#    #+#             */
/*   Updated: 2026/08/29 10:13:57 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_info
{
	int		num_coders;
	int		time_burnout;
	int		time_compile;
	int		time_debug;
	int		time_refactor;
	int		num_compiles;
	int		dongle_cooldown;
	char	*scheduler;
}			t_info;

int			is_valid_argument(char *arg, int schedular);
t_info		*parser(char **args);
#endif
