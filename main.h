/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msbii <msbii@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 09:51:30 by msbii             #+#    #+#             */
/*   Updated: 2026/09/05 12:32:18 by msbii            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_coder;
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
	long			*dongles_status;
	int				*queue;
	int				queue_size;
	int				simulation_running;
	int				request_count;
	long			start_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	simulation_mutex;
}					t_data;

typedef struct s_coder
{
	int				coder_id;
	long			last_compile_time;
	int				compile_count;
	int				request_order;
	pthread_t		thread;
	t_data			*data;
	pthread_mutex_t	state_mutex;
}					t_coder;

int					is_valid_argument(char *arg, int schedular);
t_data				*parser(char **args);
t_coder				*init_coders(t_data *data);
int					setup_env(t_data *data, t_coder *coders);
void				push_heap(t_data *data, t_coder *coder);
long				get_current_time(void);
void				*coder_wait_for_turn(t_data *data, t_coder *coder);
void				coder_compile(t_data *data, t_coder *coder);
void				coder_rest(t_data *data, t_coder *coder);
void				coder_release(t_data *data, t_coder *coder);
void				sync_coder_request(t_data *data, t_coder *coder);
void				*routine(void *arg);
void				*monitor_routine(void *arg);
void				start_simulation(t_data *data);
int					dongles_free(t_coder *coder, t_data *data);
int					is_priority(t_data *data, int id_1, int id_2);
void				extract_from_heap(t_data *data, int coder_id);
int					can_take_dongles(t_data *data, t_coder *coder);
void				precise_usleep(long time_in_ms, t_data *data);
void				print_status(t_data *data, int id, char *status);
void				clean_exit(t_data *data);
void				order_dongles(int idx1, int idx2, int *first, int *second);
int					get_sim_status(t_data *data);
#endif
