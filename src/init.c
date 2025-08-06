#include "philo.h"

// Aloca memória para os filósofos e garfos
static int	allocate_memory(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (print_error("Falha ao alocar memória para filósofos."));
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
	{
		free(data->philos);
		return (print_error("Falha ao alocar memória para garfos."));
	}
	return (0);
}

// Inicializa os mutexes dos garfos e os atribui aos filósofos
static void	init_philos_and_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].is_eating = false;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		if (data->philos[i].id % 2 == 0)
		{
			data->philos[i].left_fork = &data->forks[(i + 1) % data->num_philos];
			data->philos[i].right_fork = &data->forks[i];
		}
		i++;
	}
}

// Valida e converte os argumentos de entrada
static int	parse_args(t_data *data, int argc, char **argv)
{
	data->num_philos = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
		return (print_error("Os argumentos devem ser números positivos."));
	if (argc == 6)
	{
		data->num_meals = ft_atol(argv[5]);
		if (data->num_meals <= 0)
			return (print_error("Número de refeições deve ser positivo."));
		data->must_eat_count = true;
	}
	else
	{
		data->num_meals = -1;
		data->must_eat_count = false;
	}
	return (0);
}

// Função principal de inicialização
int	init_data(t_data *data, int argc, char **argv)
{
	if (parse_args(data, argc, argv) != 0)
		return (1);
	if (allocate_memory(data) != 0)
		return (1);
	data->sim_should_end = false;
	pthread_mutex_init(&data->data_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	init_philos_and_forks(data);
	return (0);
}
