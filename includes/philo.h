
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>

// Estruturas
typedef struct s_philo	t_philo;

// Estrutura principal que armazena as regras e dados da simulação
typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_meals;
	bool			must_eat_count;
	long			start_time;
	bool			sim_should_end;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	data_mutex;
	pthread_mutex_t	print_mutex;
}	t_data;

// Estrutura para cada filósofo
struct s_philo
{
	int				id;
	long			last_meal_time;
	int				meals_eaten;
	bool			is_eating;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
};

// Enum para o estado dos filósofos, para evitar o uso de strings
typedef enum e_status
{
	TAKING_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
}	t_status;

// Protótipos de Funções

// init.c
int		init_data(t_data *data, int argc, char **argv);

// utils.c
long	ft_atol(const char *str);
long	get_time_ms(void);
void	precise_usleep(long ms);
void	print_status(t_philo *philo, t_status status);
int		print_error(char *message);

// monitor.c
void	*monitor_routine(void *arg);

// actions.c
void	*philo_routine(void *arg);

#endif
