#include "philo.h"

// Converte uma string para um long, similar a atoi
long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

// Retorna o tempo atual em milissegundos
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// Uma implementação mais precisa de usleep
void	precise_usleep(long ms)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < ms)
		usleep(500);
}

// Imprime o status de um filósofo
void	print_status(t_philo *philo, t_status status)
{
	long	timestamp;
	bool	sim_should_end;

	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->data_mutex);
	sim_should_end = philo->data->sim_should_end;
	pthread_mutex_unlock(&philo->data->data_mutex);
	if (sim_should_end && status != DIED)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	timestamp = get_time_ms() - philo->data->start_time;
	if (status == TAKING_FORK)
		printf("%ld %d has taken a fork\n", timestamp, philo->id);
	else if (status == EATING)
		printf("%ld %d is eating\n", timestamp, philo->id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", timestamp, philo->id);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", timestamp, philo->id);
	else if (status == DIED)
		printf("%ld %d died\n", timestamp, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

// Imprime uma mensagem de erro no stderr
int	print_error(char *message)
{
	int	len;

	len = 0;
	while (message[len])
		len++;
	write(2, "Erro: ", 6);
	write(2, message, len);
	write(2, "\n", 1);
	return (1);
}
