#include "philo.h"

// Ação do filósofo de comer
static void	action_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, TAKING_FORK);
	if (philo->data->num_philos == 1)
	{
		precise_usleep(philo->data->time_to_die * 2);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKING_FORK);
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->is_eating = true;
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->data_mutex);
	print_status(philo, EATING);
	precise_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->is_eating = false;
	pthread_mutex_unlock(&philo->data->data_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

// Ação do filósofo de dormir e pensar
static void	action_sleep_think(t_philo *philo)
{
	print_status(philo, SLEEPING);
	precise_usleep(philo->data->time_to_sleep);
	print_status(philo, THINKING);
}

// Rotina principal de cada filósofo
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	bool	sim_ended;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		precise_usleep(philo->data->time_to_eat / 2);
	sim_ended = false;
	while (!sim_ended)
	{
		action_eat(philo);
		action_sleep_think(philo);
		pthread_mutex_lock(&philo->data->data_mutex);
		sim_ended = philo->data->sim_should_end;
		pthread_mutex_unlock(&philo->data->data_mutex);
	}
	return (NULL);
}
