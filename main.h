/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 09:51:30 by msbii             #+#    #+#             */
/*   Updated: 2026/09/01 20:26:19 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

struct s_coder;
typedef struct s_data
{
	int				num_coders;
	int				time_burnout;
	int				time_compile;
	int				time_debug;
	int				time_refactor;
	int				num_compiles;
	int				dongle_cooldown;
	char			*scheduler;
	struct s_coder	*coders;
	pthread_mutex_t	*dongles;
	int				*dongles_status;
	int				*queue;
	int				queue_size;
	int				simulation_running;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	queue_mutex;
	pthread_mutex_t	simulation_mutex;
}					t_data;

typedef struct s_coder
{
	int				coder_id;
	long			last_compile_time;
	int				compile_count;
	pthread_t		thread;
	pthread_cond_t	cond;
	t_data			*data;
}					t_coder;

int					is_valid_argument(char *arg, int schedular);
t_data				*parser(char **args);
t_coder				*init_coders(t_data *data);
int					setup_env(t_data *data, t_coder *coders);
#endif
