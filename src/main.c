#include "philo.h"

// Libera todos os recursos alocados
static void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->data_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}

// Inicia as threads dos filósofos e a thread do monitor
static int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	data->start_time = get_time_ms();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				&philo_routine, &data->philos[i]) != 0)
			return (print_error("Falha ao criar a thread do filósofo."));
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, &monitor_routine, data) != 0)
		return (print_error("Falha ao criar a thread do monitor."));
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (print_error("Falha ao juntar a thread do filósofo."));
		i++;
	}
	if (pthread_join(monitor_thread, NULL) != 0)
		return (print_error("Falha ao juntar a thread do monitor."));
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (print_error("Número de argumentos inválido."));
	if (init_data(&data, argc, argv) != 0)
		return (1);
	if (start_simulation(&data) != 0)
	{
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}
