#include "philo.h"

// Verifica se um filósofo morreu de fome
static bool	check_death(t_philo *philo)
{
	long	time_since_last_meal;

	if (philo->is_eating)
		return (false);
	time_since_last_meal = get_time_ms() - philo->last_meal_time;
	if (time_since_last_meal > philo->data->time_to_die)
	{
		philo->data->sim_should_end = true;
		print_status(philo, DIED);
		return (true);
	}
	return (false);
}

// Verifica se todos os filósofos comeram o número necessário de vezes
static bool	check_all_ate(t_data *data)
{
	int	i;
	int	finished_eating_count;

	i = 0;
	finished_eating_count = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten >= data->num_meals)
			finished_eating_count++;
		i++;
	}
	if (finished_eating_count == data->num_philos)
	{
		data->sim_should_end = true;
		return (true);
	}
	return (false);
}

// Rotina do monitor que supervisiona a simulação
void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	bool	sim_ended;

	data = (t_data *)arg;
	sim_ended = false;
	while (!sim_ended)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->data_mutex);
			if (check_death(&data->philos[i]))
			{
				pthread_mutex_unlock(&data->data_mutex);
				return (NULL);
			}
			if (data->must_eat_count && check_all_ate(data))
			{
				pthread_mutex_unlock(&data->data_mutex);
				return (NULL);
			}
			sim_ended = data->sim_should_end;
			pthread_mutex_unlock(&data->data_mutex);
			i++;
		}
	}
	return (NULL);
}
